//
// Created by 陶子杨 on 25-10-23.
//

#include "Lexer/C0Lexer.h"

namespace CC {

      std::unordered_map<std::string, TokenType> C0Lexer::keywords = {
        {"int", TokenType::KW_INT},
        {"bool", TokenType::KW_BOOL},
        {"void", TokenType::KW_VOID},
        {"if", TokenType::KW_IF},
        {"else", TokenType::KW_ELSE},
        {"while", TokenType::KW_WHILE},
        {"return", TokenType::KW_RETURN},
        {"true", TokenType::KW_TRUE},
        {"false", TokenType::KW_FALSE},
        {"struct", TokenType::KW_STRUCT},
        {"typedef", TokenType::KW_TYPEDEF},
        {"for", TokenType::KW_FOR},
        {"continue", TokenType::KW_CONTINUE},
        {"break", TokenType::KW_BREAK},
        {"assert", TokenType::KW_ASSERT},
        {"NULL", TokenType::KW_NULL},
        {"alloc", TokenType::KW_ALLOC},
        {"alloc_array", TokenType::KW_ALLOC_ARRAY},
        {"char", TokenType::KW_CHAR},
        {"string", TokenType::KW_STRING}
    };

    C0Lexer::C0Lexer(const std::string& file_path) {
        code_manager = std::make_unique<CodeManager>(file_path);

        //预处理，消除所有的注释
        std::vector<char> result;
        while (!code_manager->eofReached()) {
            char c = code_manager->getChar();
            if (c == '/') {
                char next = code_manager->getChar();
                if (next == '/') {
                    //单行注释
                    while (!code_manager->eofReached() && code_manager->getChar() != '\n');
                }
                else if (next == '*') {
                    //多行注释
                    char last = code_manager->getChar();
                    while (!code_manager->eofReached()) {
                        c = code_manager->getChar();
                        if (last == '*' && c == '/') {
                            break;
                        }
                        last = c;
                    }
                }
                else {
                    result.push_back(c);
                }
            }
            else {
                result.push_back(c);
            }
        }
        code_manager->updateBuffer(result);
        code_manager->restar();
    }

    Token C0Lexer::nextToken() {
        skipWhitespace();

        if (code_manager->eofReached()) {
            return {TokenType::END_OF_FILE, "", code_manager->getLocation()};
        }

        char c = code_manager->lookChar();

        if (isDigit(c)) {
            return {TokenType::INT_LITERAL, readNumber(), code_manager->getLocation()};
        }

        if (isLetter(c) || c == '_') {
            return readKeywordOrIdentifier();
        }

        if (isOperator(c)) {
            return readOperator();
        }

        if (isDelimiter(c)) {
            return readDelimiter();
        }

        if (c == '"') {
            return readString();
        }

        if (c == '\'') {
            return {TokenType::CHAR_LITERAL, readChar(), code_manager->getLocation()};
        }

        return {TokenType::UNKNOWN, std::string(1,code_manager->getChar()), code_manager->getLocation()};
    }

    bool C0Lexer::isKeyword(const std::string& word) {
        return keywords.find(word) != keywords.end();
    }

    Token C0Lexer::readKeywordOrIdentifier() {
        std::string word;
        while (isLetter(code_manager->lookChar()) ||
            isDigit(code_manager->lookChar()) ||
            code_manager->lookChar() == '_') {
            word += code_manager->getChar();
        }

        if (isKeyword(word)) {
            return {keywords[word], word, code_manager->getLocation()};
        } else {
            return {TokenType::IDENTIFIER, word, code_manager->getLocation()};
        }
    }

    Token C0Lexer::readOperator() {
        std::string word;

        char c = code_manager->getChar();
        word = c;
        char next = code_manager->getChar();
        word += next;
        if (word == "+=") {
            return {TokenType::OP_PLUS_ASSIGN, word, code_manager->getLocation()};
        } else if (word == "-=") {
            return {TokenType::OP_MINUS_ASSIGN, word, code_manager->getLocation()};
        } else if (word == "*=") {
            return {TokenType::OP_MULTIPLY_ASSIGN, word, code_manager->getLocation()};
        } else if (word == "\\=") {
            return {TokenType::OP_DIVIDE_ASSIGN, word, code_manager->getLocation()};
        } else if (word == "%=") {
            return {TokenType::OP_MODULO_ASSIGN, word, code_manager->getLocation()};
        } else if (word == "==") {
            return {TokenType::OP_EQ, word, code_manager->getLocation()};
        } else if (word == "!=") {
            return {TokenType::OP_NE, word, code_manager->getLocation()};
        } else if (word == "<=") {
            return {TokenType::OP_LE, word, code_manager->getLocation()};
        } else if (word == ">=") {
            return {TokenType::OP_GE, word, code_manager->getLocation()};
        } else if (word == "&&") {
            return {TokenType::OP_LOGICAL_AND, word, code_manager->getLocation()};
        } else if (word == "||") {
            return {TokenType::OP_LOGICAL_OR, word, code_manager->getLocation()};
        }
        code_manager->ungetChar();
        switch (c) {
        case '+':
            return {TokenType::OP_PLUS, std::string(1, c), code_manager->getLocation()};
        case '-':
            return {TokenType::OP_MINUS, std::string(1, c), code_manager->getLocation()};
        case '*':
            return {TokenType::OP_MULTIPLY, std::string(1, c), code_manager->getLocation()};
        case '/':
            return {TokenType::OP_DIVIDE, std::string(1, c), code_manager->getLocation()};
        case '%':
            return {TokenType::OP_MODULO, std::string(1, c), code_manager->getLocation()};
        case '=':
            return {TokenType::OP_ASSIGN, std::string(1, c), code_manager->getLocation()};
        case '>':
            return {TokenType::OP_GT, std::string(1, c), code_manager->getLocation()};
        case '<':
            return {TokenType::OP_LT, std::string(1, c), code_manager->getLocation()};
        case '!':
            return {TokenType::OP_NOT, std::string(1, c), code_manager->getLocation()};
        case '&':
            return {TokenType::OP_AND, std::string(1, c), code_manager->getLocation()};
        case '|':
            return {TokenType::OP_OR, std::string(1, c), code_manager->getLocation()};
        case '^':
            return {TokenType::OP_XOR, std::string(1, c), code_manager->getLocation()};
        }
    }

    Token C0Lexer::readDelimiter() {
        char c = code_manager->getChar();
        switch (c) {
        case ';':
            return {TokenType::SEMICOLON, ";", code_manager->getLocation()};
        case ',':
            return {TokenType::COMMA, ",", code_manager->getLocation()};
        case '(':
            return {TokenType::LPAREN, "(", code_manager->getLocation()};
        case ')':
            return {TokenType::RPAREN, ")", code_manager->getLocation()};
        case '{':
            return {TokenType::LBRACE, "{", code_manager->getLocation()};
        case '}':
            return {TokenType::RBRACE, "}", code_manager->getLocation()};
        case '.':
            return {TokenType::DOT, ".", code_manager->getLocation()};
        }
    }
}
