//
// Created by 陶子杨 on 25-11-4.
//

#include "Parser/C0Parser.h"

#include "Infra/casting.h"

namespace CC {
    std::shared_ptr<Expression> C0Parser::parseExpression(int minPrec) {
        std::shared_ptr<Expression> left = parsePrefixExpression();
        while (true) {
            Token op = peek(1);
            int prec = getInfixPrecedence(op.type);

            // 不是中缀运算符 或者 优先级低于当前 minPrec，则停止
            if (prec < minPrec || prec == 0) {
                break;
            }

            // 处理结合性：右结合的右边用相同优先级，左结合的右边用 prec + 1
            int nextMinPrec = isRightAssociative(op.type) ? prec : prec + 1;

            advance(1); // 消耗运算符

            // 右操作数递归调用 parseExpression
            auto right = parseExpression(nextMinPrec);

            // 根据是赋值还是普通二元，构建不同节点
            if (op.type == TokenType::OP_ASSIGN) {
                left = std::make_shared<AssignmentExpr>(left, right,op.type); // ASSIGNMENT_EXPR
            } else {
                left = std::make_shared<BinaryExpr>(left, right, op.type); // BINARY_EXPR
            }
        }

        return left;
    }

    std::shared_ptr<Expression> C0Parser::parsePrefixExpression() {
        Token token = advance(1);
        switch (token.type) {
        //一元表达式
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:
        case TokenType::OP_NOT: {
            auto operand  = parsePrefixExpression();
            return std::make_shared<UnaryExpr>(operand ,token.type);
        }
        //字面量常量
        case TokenType::INT_LITERAL:
            return std::make_shared<IntegerLiteralExpr>(std::stoi(token.lexeme));
        case TokenType::CHAR_LITERAL:
            return std::make_shared<CharLiteralExpr>(token.lexeme[0]);
        case TokenType::STRING_LITERAL:
            return std::make_shared<StringLiteralExpr>(token.lexeme);
        case TokenType::BOOL_LITERAL:
            return std::make_shared<BoolLiteralExpr>(token.lexeme == "true");
        case TokenType::IDENTIFIER:
            return std::make_shared<IdentifierExpr>(token.lexeme);
        case TokenType::LPAREN: {
            auto expr = parseExpression();
            Token rparne = advance(1);
            return rparne.type == TokenType::RPAREN ? expr : nullptr;
        }
        default:
            //TODO: 错误处理
            return nullptr;
        }
    }

    int C0Parser::getInfixPrecedence(TokenType type) {
        switch (type) {
        case TokenType::OP_ASSIGN:
            return 1;  // =
        case TokenType::OP_OR:
            return 2;  // ||
        case TokenType::OP_AND:
            return 3;  // &&
        case TokenType::OP_EQ:
        case TokenType::OP_NE:
            return 4;  // == !=
        case TokenType::OP_LT:
        case TokenType::OP_GT:
        case TokenType::OP_LE:
        case TokenType::OP_GE:
            return 5;  // < > <= >=
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:
            return 6;  // + -
        case TokenType::OP_MULTIPLY:
        case TokenType::OP_DIVIDE:
        case TokenType::OP_MODULO:
            return 7;  // * / %
        default:
            return 0;  // 0 表示“不是中缀运算符”
        }
    }

    bool C0Parser::isRightAssociative(TokenType type) {
        // 比如赋值、指数这些是右结合的
        switch (type) {
        case TokenType::OP_ASSIGN:
        case TokenType::OP_MULTIPLY_ASSIGN:
        case TokenType::OP_DIVIDE_ASSIGN:
        case TokenType::OP_MODULO_ASSIGN:
        case TokenType::OP_PLUS_ASSIGN:
        case TokenType::OP_MINUS_ASSIGN:
        case TokenType::OP_LOGICAL_AND:
        case TokenType::OP_LOGICAL_OR:
        case TokenType::OP_XOR:
            return true;
        default:
            return false;
        }
    }

    std::shared_ptr<Declaration> C0Parser::parseDeclaration() {
        Token mark = peek(3);
        if (mark.type == TokenType::LPAREN) {
            return parseFunctionDeclaration();
        }
        else if (mark.type == TokenType::END_OF_FILE) {
            return nullptr;
        }
        return parseVariableDeclaration();
    }

    std::shared_ptr<Declaration> C0Parser::parseFunctionDeclaration() {
        Token type = advance(1);
        Token name = advance(1);
        advance(1); // 跳过 '('

        std::vector<std::shared_ptr<Declaration>> params;
        while (peek(1).type != TokenType::RPAREN) {
            auto param = parseParamDecl(); // 只吃 "type name" 和可能的逗号
            params.push_back(param);
        }
        if (peek(1).type == TokenType::RPAREN) {
            advance(1);
        } else {
            // TODO: 错误处理
        }
        std::shared_ptr<Statement> body = parseCompoundStmt();
        return std::make_shared<FunctionDecl>(
            name.lexeme,
            type.lexeme,
            params,
            INFRA::dyn_cast<CompoundStmt>(body)
        );
    }

    std::shared_ptr<Declaration> C0Parser::parseParamDecl() {
        Token type = advance(1);
        Token name = advance(1);
        if (peek(1).type == TokenType::COMMA) {
            advance(1);
        }
        else {
            //TODO: 错误处理
        }
        return std::make_shared<VariableDecl>(name.lexeme, type.lexeme, nullptr);
    }

    std::shared_ptr<Declaration> C0Parser::parseVariableDeclaration() {
        Token type = advance(1);
        Token name = advance(1);

        std::shared_ptr<Expression> initializer = nullptr;

        if (peek(1).type == TokenType::OP_EQ) {
            advance(1);
            initializer = parseExpression();
        }

        // 此时当前 token 应该是 ';' 或 ',' 或 ')'
        Token endTok = peek(1);
        switch (endTok.type) {
        case TokenType::SEMICOLON:
        case TokenType::COMMA:
        case TokenType::RPAREN:
            advance(1); // 吃掉结束符号
            break;
        default:
            // 以后可以在这里做错误处理
            break;
        }
        return std::make_shared<VariableDecl>(name.lexeme, type.lexeme, initializer);
    }

    std::shared_ptr<Statement> C0Parser::parseStatement() {
        Token token = peek(1);
        switch (token.type) {
        case TokenType::LBRACE:
            return parseCompoundStmt();
        case TokenType::KW_IF:
            return parseIfStmt();
        case TokenType::KW_ELSE:
            return parseElseStmt();
        case TokenType::KW_FOR:
            return parseForStmt();
        case TokenType::KW_WHILE:
            return parseWhileStmt();
        case TokenType::KW_DO:
            return parseDowhileStmt();
        case TokenType::KW_CONTINUE:
            if (peek(1).type != TokenType::SEMICOLON) {
                //TODO: 错误处理
            }
            return std::make_shared<ContinueStmt>();
        case TokenType::KW_BREAK:
            if (peek(1).type != TokenType::SEMICOLON) {
                //TODO: 错误处理
            }
            return std::make_shared<BreakStmt>();
        case TokenType::KW_RETURN:
            return parseReturnStmt();
        default:
            break;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> C0Parser::parseCompoundStmt() {
        Token lbrace = peek(1);
        if (lbrace.type != TokenType::LBRACE) {
            //TODO: 错误处理
        }
        else {
            advance(1);
        }
        std::vector<std::shared_ptr<Statement>> statements;
        while (peek(1).type != TokenType::RBRACE && peek(1).type != TokenType::END_OF_FILE) {
            auto stmt = parseStatement();
            statements.push_back(stmt);
        }
        if (advance(1).type != TokenType::RBRACE) {
            //TODO: 错误处理
        }

        return std::make_shared<CompoundStmt>(std::move(statements));
    }

    std::shared_ptr<Statement> C0Parser::parseIfStmt() {

    }

    std::shared_ptr<Statement> C0Parser::parseElseStmt() {

    }

    std::shared_ptr<Statement> C0Parser::parseWhileStmt() {

    }

    std::shared_ptr<Statement> C0Parser::parseForStmt() {

    }

    std::shared_ptr<Statement> C0Parser::parseDowhileStmt() {

    }

    std::shared_ptr<Statement> C0Parser::parseReturnStmt() {

    }
}
