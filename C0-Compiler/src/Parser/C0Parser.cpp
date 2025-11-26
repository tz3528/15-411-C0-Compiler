//
// Created by 陶子杨 on 25-11-4.
//

#include "Parser/C0Parser.h"
#include "AST/StatementNode.h"
#include "Infra/casting.h"

namespace CC {
    std::shared_ptr<Expression> C0Parser::parseExpression(int minPrec) {
        std::shared_ptr<Expression> left = parsePrefixExpression();
        while (true) {
            Token op = peek(0);
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
                left = std::make_shared<AssignmentExpr>(left, right,op.type);
            } else {
                left = std::make_shared<BinaryExpr>(left, right, op.type);
            }
        }

        return left;
    }

    std::shared_ptr<Expression> C0Parser::parsePrefixExpression() {
        Token token = peek(0);

        // 如果是一元运算符 (+, -, !)
        if (token.type == TokenType::OP_PLUS ||
            token.type == TokenType::OP_MINUS ||
            token.type == TokenType::OP_NOT) {

            advance(1); // 消耗运算符

            // 递归调用 parsePrefixExpression，支持 !!x 或 - -y
            auto operand = parsePrefixExpression();
            return std::make_shared<UnaryExpr>(operand, token.type);
            }

        // 如果不是一元运算符，下沉到后缀表达式层级
        return parsePostfixExpression();
    }

    std::shared_ptr<Expression> C0Parser::parsePostfixExpression() {
        // 先解析最基本的元素 (标识符、字面量、括号表达式)
        auto expr = parsePrimary();

        // 循环检查后面是否紧跟着后缀运算符
        while (true) {
            Token t = peek(0);

            // --- Case A: 函数调用 '(' ---
            if (t.type == TokenType::LPAREN) {
                advance(1); // 消耗 '('

                std::vector<std::shared_ptr<Expression>> args;

                // 如果不是立即闭合 ')'，说明有参数
                if (peek(0).type != TokenType::RPAREN) {
                    args.push_back(parseExpression()); // 解析第一个参数

                    // 解析后续参数 (被 ',' 分隔)
                    while (match(TokenType::COMMA)) {
                        args.push_back(parseExpression());
                    }
                }

                if (!match(TokenType::RPAREN)) {
                    // TODO: 报错 "Expect ')'
                    return nullptr;
                }

                // 将当前的 expr 包装进 CallExpr，并更新 expr
                // 这样支持链式调用，如 getFunc()(arg)
                expr = std::make_shared<CallExpr>(expr, std::move(args));
            }

            // --- 既不是调用也不是下标，后缀解析结束 ---
            else {
                break;
            }
        }

        return expr;
    }

    std::shared_ptr<Expression> C0Parser::parsePrimary() {
        Token token = advance(1); // 获取并消耗当前 Token

        switch (token.type) {
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
            // 处理括号中的表达式 (expr)
            auto expr = parseExpression();
            if (peek(0).type != TokenType::RPAREN) {
                // TODO: 错误处理 "Expect ')'"
                return nullptr;
            }
            else {
                advance(1); // 消耗 ')'
            }
            return expr;
        }

        default:
            // TODO: 错误处理
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
        if (peek(0).type == TokenType::KW_STRUCT) {
            return parseStructDeclaration();
        }
        Token mark = peek(2);
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

        std::vector<std::shared_ptr<VariableDecl>> params;
        while (peek(0).type != TokenType::RPAREN) {
            auto param = parseParamDecl(); // 只吃 "type name" 和可能的逗号
            params.push_back(param);
        }
        if (peek(0).type == TokenType::RPAREN) {
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

    std::shared_ptr<VariableDecl> C0Parser::parseParamDecl() {
        Token type = advance(1);
        Token name = advance(1);
        if (peek(0).type == TokenType::COMMA) {
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

        if (peek(0).type == TokenType::OP_EQ) {
            advance(1);
            initializer = parseExpression();
        }

        // 此时当前 token 应该是 ';' 或 ',' 或 ')'
        Token endTok = peek(0);
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

    std::shared_ptr<Declaration> C0Parser::parseStructDeclaration() {
        advance(1); // 吃掉 'struct'
        Token name = advance(1);
        advance(1); // 吃掉 '{'
        std::vector<std::shared_ptr<VariableDecl>> members;
        while (peek(0).type != TokenType::RBRACE) {
            auto member = parseVariableDeclaration();
            if (auto memberDecl = INFRA::dyn_cast<VariableDecl>(member)) {
                members.push_back(std::move(memberDecl));
            }
        }
        advance(1); // 吃掉 '}'
        advance(1); // 吃掉 ';'
        return std::make_shared<StructDecl>(name.lexeme, members);
    }

    std::shared_ptr<Statement> C0Parser::parseStatement() {
        Token token = peek(0);
        switch (token.type) {
        case TokenType::LBRACE: {
            auto compoundStmt = parseCompoundStmt();
            return compoundStmt;
        }
        case TokenType::KW_IF:
            return parseIfStmt();
        case TokenType::KW_FOR:
            return parseForStmt();
        case TokenType::KW_WHILE:
            return parseWhileStmt();
        case TokenType::KW_DO:
            return parseDowhileStmt();
        case TokenType::KW_CONTINUE:
            advance(1);
            if (peek(0).type != TokenType::SEMICOLON) {
                //TODO: 错误处理
            }
            else {
                advance(1);
            }
            return std::make_shared<ContinueStmt>();
        case TokenType::KW_BREAK:
            advance(1);
            if (peek(0).type != TokenType::SEMICOLON) {
                //TODO: 错误处理
            }
            else {
                advance(1);
            }
            return std::make_shared<BreakStmt>();
        case TokenType::KW_RETURN:
            return parseReturnStmt();
        case TokenType::SEMICOLON:
            // 空语句
            advance(1);
            return std::make_shared<NullStmt>();
        default:
            break;
        }

        if (isTypeSpecifier(token)) {
            return std::make_shared<DeclStmt>(parseDeclaration());
        }
        auto expr = parseExpression();
        if (peek(0).type != TokenType::SEMICOLON) {
            //TODO: 错误处理
        }
        else {
            advance(1);
        }
        return std::make_shared<ExpressionStmt>(expr);
    }

    std::shared_ptr<Statement> C0Parser::parseCompoundStmt() {
        Token lbrace = peek(0);
        if (lbrace.type != TokenType::LBRACE) {
            //TODO: 错误处理
        }
        else {
            advance(1);
        }
        std::vector<std::shared_ptr<Statement>> statements;
        while (peek(0).type != TokenType::RBRACE && peek(0).type != TokenType::END_OF_FILE) {
            auto stmt = parseStatement();
            statements.push_back(stmt);
        }
        if (peek(0).type != TokenType::RBRACE) {
            //TODO: 错误处理
        }
        advance(1);

        return std::make_shared<CompoundStmt>(std::move(statements));
    }

    std::shared_ptr<Statement> C0Parser::parseIfStmt() {
        Token token = peek(0);
        if (token.type != TokenType::KW_IF) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 'if'
        if (peek(0).type != TokenType::LPAREN) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 '('
        auto expr = parseExpression();
        if (peek(0).type != TokenType::RPAREN) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 ')'
        auto stmt = parseStatement();
        std::shared_ptr<Statement> elseStmt = nullptr;
        if (peek(0).type == TokenType::KW_ELSE) {
            advance(1); // 吃掉 'else'
            elseStmt = parseStatement();
        }
        return std::make_shared<IfStmt>(expr, stmt, elseStmt);
    }

    std::shared_ptr<Statement> C0Parser::parseWhileStmt() {
        Token token = peek(0);
        if (token.type != TokenType::KW_WHILE) {
            //TODO: 错误处理
        }
        advance(1);
        if (peek(0).type != TokenType::LPAREN) {
            //TODO: 错误处理
        }
        advance(1);
        auto expr = parseExpression();
        if (peek(0).type != TokenType::RPAREN) {
            //TODO: 错误处理
        }
        advance(1);
        auto stmt = parseStatement();
        return std::make_shared<WhileStmt>(expr, stmt);
    }

    std::shared_ptr<Statement> C0Parser::parseForStmt() {
        Token token = peek(0);
        if (token.type != TokenType::KW_FOR) {
            //TODO: 错误处理
        }
        advance(1);
        if (peek(0).type != TokenType::LPAREN) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 '('

        // 1. init 部分：可以是声明、表达式或空
        std::shared_ptr<Statement> init = nullptr;
        if (peek(0).type != TokenType::SEMICOLON) {
            if (isTypeSpecifier(peek(0))) {
                init = std::make_shared<DeclStmt>(parseDeclaration());
            } else {
                init = std::make_shared<ExpressionStmt>(parseExpression());
            }
        }
        if (peek(0).type != TokenType::SEMICOLON) {
            // TODO: 错误处理
        }
        advance(1); // 吃掉 ';'

        // 2. cond 部分：表达式或空
        std::shared_ptr<Expression> cond = nullptr;
        if (peek(0).type != TokenType::SEMICOLON) {
            cond = parseExpression();
        }
        if (peek(0).type != TokenType::SEMICOLON) {
            // TODO: 错误处理
        }
        advance(1); // 吃掉 ';'

        // 3. step 部分：表达式或空
        std::shared_ptr<Expression> step = nullptr;
        if (peek(0).type != TokenType::RPAREN) {  // 可以为空
            step = parseExpression();
        }

        if (peek(0).type != TokenType::RPAREN) {
            // TODO: 错误处理
        }
        advance(1); // 吃掉 ')'


        auto stmt = parseStatement();
        return std::make_shared<ForStmt>(init, cond, step, stmt);
    }

    std::shared_ptr<Statement> C0Parser::parseDowhileStmt() {
        Token token = peek(0);
        if (token.type != TokenType::KW_DO) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 'do'
        auto stmt = parseStatement();
        if (peek(0).type != TokenType::KW_WHILE) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 'while'
        if (peek(0).type != TokenType::LPAREN) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 '('
        auto expr = parseExpression();
        if (peek(0).type != TokenType::RPAREN) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 ')'
        if (peek(0).type != TokenType::SEMICOLON) {
            //TODO: 错误处理
        }
        advance(1); // 吃掉 ';'
        return std::make_shared<DoWhileStmt>(stmt, expr);
    }

    std::shared_ptr<Statement> C0Parser::parseReturnStmt() {
        Token token = peek(0);
        if (token.type != TokenType::KW_RETURN) {
            //TODO: 错误处理
        }
        advance(1);
        std::shared_ptr<Expression> expr = nullptr;
        if (peek(0).type != TokenType::SEMICOLON) {
            expr = parseExpression();
        }
        if (peek(0).type != TokenType::SEMICOLON) {
            //TODO: 错误处理
        }
        advance(1);
        return std::make_shared<ReturnStmt>(expr);
    }
}
