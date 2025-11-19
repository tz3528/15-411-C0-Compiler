//
// Created by 陶子杨 on 25-11-4.
//

#pragma once

#include "AST/UnitNode.h"
#include "Lexer/C0Lexer.h"
#include "Parser/parser.h"

namespace CC {
    class C0Parser : public Parser<C0Parser> {
    public:
        /**
         * @brief 构造函数，初始化词法分析器并读取所有token
         * @param file_path 要解析的源文件路径
         */
        explicit C0Parser(const std::string& file_path) {
            lexer = std::make_unique<C0Lexer>(file_path);
            while (true) {
                tokens_.push_back(lexer->nextToken());
                if (tokens_.back().type == TokenType::END_OF_FILE) {
                    break;
                }
            }
        }

        /**
         * @brief 解析整个程序，生成抽象语法树
         */
        void parse() {
            while (position < tokens_.size()) {
                AST_root->declarations.push_back(parseDeclaration());
            }
        }
    private:
        /**
         * @brief 查看向前k个位置的token，不移动当前位置
         * @param k 向前查看的位置数
         * @return 对应位置的token
         */
        Token peek(int k) {
            if (position + k >= tokens_.size()) {
                return {TokenType::END_OF_FILE, "EOF", Location()};
            }
            return tokens_[position + k];
        }

        /**
         * @brief 向前移动k个位置并返回新位置的token
         * @param k 移动的位置数，默认为1
         * @return 新位置的token
         */
        Token advance(int k = 1) {
            position = position + k;
            return tokens_[position];
        }

        /**
         * @brief 解析表达式
         * @return 表达式的AST节点
         */
        std::shared_ptr<Expression> parseExpression(int minPrec = 0);

        std::shared_ptr<Expression> parsePrefixExpression();

        static int getInfixPrecedence(TokenType type);

        static bool isRightAssociative(TokenType type);

        /**
         * @brief 解析语句
         * @return 语句的AST节点
         */
        std::shared_ptr<Statement> parseStatement();

        /**
         * 通过看第三个符号来判断当前声明语句的类型
         * @return 返回当前的声明语句
         */
        std::shared_ptr<Declaration> parseDeclaration();

        /**
         *
         * @return 返回函数声明语句
         */
        std::shared_ptr<Declaration> parseFunctionDeclaration();
        
        /**
         * @brief 解析参数声明
         * @return 参数声明的AST节点
         */
        std::shared_ptr<Declaration> parseParamDecl();
        
        /**
         * @brief 解析变量声明
         * @return 变量声明的AST节点
         */
        std::shared_ptr<Declaration> parseVariableDeclaration();

        /**
         * @brief 解析复合语句（代码块）
         * @return 复合语句的AST节点
         */
        std::shared_ptr<CompoundStmt> parseCompoundStmt();

        std::unique_ptr<C0Lexer> lexer;           ///< 词法分析器
        std::unique_ptr<TranslationUnit> AST_root;///< 抽象语法树根节点
        std::vector<Token> tokens_;               ///< token序列
        size_t position = 0;                      ///< 当前解析位置
    };
}