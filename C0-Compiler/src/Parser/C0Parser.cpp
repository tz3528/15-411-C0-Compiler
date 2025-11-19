//
// Created by 陶子杨 on 25-11-4.
//

#include "Parser/C0Parser.h"

namespace CC {
    std::shared_ptr<Expression> C0Parser::parseExpression() {
        Token token =peek(1);
        switch (token.type) {

        default:
            break;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> C0Parser::parseStatement() {

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
        std::shared_ptr<CompoundStmt> body = parseCompoundStmt();
        return std::make_shared<FunctionDecl>(name.lexeme, type.lexeme, params, body);
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

    std::shared_ptr<CompoundStmt> C0Parser::parseCompoundStmt() {

    }
}