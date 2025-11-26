//
// Created by 陶子杨 on 25-10-23.
//

#pragma once

#include "CodeManager/CodeManager.h"

#include <string>
#include <memory>

namespace CC {
    enum class TokenType {
        // 关键字
        KW_INT, KW_BOOL, KW_VOID, KW_IF, KW_ELSE, KW_WHILE,
        KW_RETURN, KW_TRUE, KW_FALSE, KW_STRUCT, KW_TYPEDEF,
        KW_FOR, KW_CONTINUE, KW_BREAK, KW_NULL,
        KW_ALLOC, KW_ALLOC_ARRAY, KW_CHAR, KW_STRING, KW_DO,

        // 标识符
        IDENTIFIER,

        // 字面量
        INT_LITERAL, BOOL_LITERAL, STRING_LITERAL, CHAR_LITERAL,

        // 运算符
        OP_PLUS, OP_MINUS, OP_MULTIPLY, OP_DIVIDE, OP_MODULO,
        OP_ASSIGN, OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
        OP_AND, OP_OR, OP_NOT, OP_XOR,

        // 复合赋值运算符
        OP_PLUS_ASSIGN, OP_MINUS_ASSIGN, OP_MULTIPLY_ASSIGN,
        OP_DIVIDE_ASSIGN, OP_MODULO_ASSIGN,
        // 逻辑运算符
        OP_LOGICAL_AND, OP_LOGICAL_OR,

        // 分隔符
        SEMICOLON, COMMA, DOT,
        LPAREN, RPAREN, LBRACE, RBRACE,

        // 特殊符号
        END_OF_FILE, UNKNOWN,
    };

    struct Token {
        TokenType type;
        std::string lexeme;      // 原始的字符串
        Location location;

        // 对于字面量，存储解析后的值
        union {
            int int_value;
            bool bool_value;
        };
    };

    template<typename Derived>
    class Lexer {
    public:
        Token nextToken() {
            return static_cast<Derived*>(this)->nextToken();
        }

    protected:
        void skipWhitespace() {
            while (!code_manager->eofReached()) {
                char c = code_manager->lookChar();
                if (c == ' ' ||
                    c == '\t' ||
                    c == '\n' ||
                    c == '\r' ||
                    c == '\f' ||
                    c == '\v' ||
                    c == '\0'
                        ) {
                    code_manager->getChar();
                }
                else {
                    break;
                }
            }
        }
        
        static bool isDigit(const char& ch) {
            return ch >= '0' && ch <= '9';
        }

        static bool isLetter(const char& ch) {
            return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
        }

        static bool isOperator(const char& ch) {
            switch (ch) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                case '=':
                case '!':
                case '<':
                case '>':
                case '&':
                case '|':
                case '^':
                    return true;
                default:
                    return false;
            }
        }

        static bool isDelimiter(const char& ch) {
            switch (ch) {
                case ';':
                case ',':
                case '(':
                case ')':
                case '{':
                case '}':
                    return true;
                default:
                    return false;
            }
        }

        [[nodiscard]] std::string readNumber() const  {
            std::string number;
            while (isDigit(code_manager->lookChar())) {
                number += code_manager->getChar();
            }
            return number;
        }

        Token readString() const {
            std::string string;
            code_manager->getChar();
            while (code_manager->lookChar() != '"' && !code_manager->eofReached()) {
                char ch = code_manager->getChar();
                if (ch == '\\') { // 处理转义字符
                    char nextCh = code_manager->getChar();
                    switch(nextCh) {
                    case 'n': string += '\n'; break;
                    case 't': string += '\t'; break;
                    case '"': string += '"'; break;
                        // 可以添加更多转义字符支持
                    default: string += nextCh; break;
                    }
                } else if (ch == '\n') {
                    return {TokenType::UNKNOWN, "unknown", code_manager->getLocation()};
                } else {
                    string += ch;
                }
            }
            if (code_manager->eofReached()) {
                return {TokenType::UNKNOWN, "unknown", code_manager->getLocation()};
            }
            code_manager->getChar();
            return {TokenType::STRING_LITERAL, string, code_manager->getLocation()};
        }

        std::string readChar() const {
            code_manager->getChar();
            std::string c(1, code_manager->getChar());
            code_manager->getChar();
            return c;
        }


        std::unique_ptr<CodeManager> code_manager;
    };


}