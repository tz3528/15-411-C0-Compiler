//
// Created by 陶子杨 on 25-10-23.
//
#pragma once

#include "Lexer/Lexer.h"

#include <unordered_map>

namespace CC {
    class C0Lexer : public Lexer<C0Lexer> {
    public:
        explicit C0Lexer(const std::string& file_path) ;

        Token nextToken();
    private:
        static std::unordered_map<std::string, TokenType> keywords;

        static bool isKeyword(const std::string& word);

        Token readKeywordOrIdentifier();

        Token readOperator();

        Token readDelimiter();
    };
}