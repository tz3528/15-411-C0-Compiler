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

    /**
     * @brief 解析声明语句
     * 
     * 通过查看第3个token来判断是函数声明还是变量声明：
     * - 如果第3个token是左括号'('，则是函数声明
     * - 否则是变量声明
     * 
     * @return 返回解析得到的声明节点
     */
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

    /**
     * @brief 解析函数声明
     * 
     * 函数声明格式：返回类型 函数名(参数列表) {函数体}
     * 例如：int func(int a, int b) { ... }
     * 
     * @return 返回函数声明节点
     */
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

    /**
     * @brief 解析参数声明
     * 
     * 参数声明格式：类型 名称
     * 例如：int a
     * 
     * @return 返回参数声明节点
     */
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

    /**
     * @brief 解析变量声明
     * 
     * 变量声明格式：类型 变量名 [= 初始值];
     * 例如：int a; 或 int b = 10;
     * 
     * @return 返回变量声明节点
     */
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