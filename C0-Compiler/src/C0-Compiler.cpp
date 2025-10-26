//
// Created by 陶子杨 on 25-10-23.
//
#include "C0-Compiler.h"
#include "Lexer/Lexer.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Lexer/C0Lexer.h"

int main(int argc, char* argv[]) {
    // 检查是否有输入参数
    if (argc < 2) {
        std::cout << "用法: " << argv[0] << " <参数>" << std::endl;
        std::cout << "可用参数:" << std::endl;
        std::cout << "  compile <输入文件>    编译指定的C0源文件" << std::endl;
        std::cout << "  help                 显示帮助信息" << std::endl;
        return 1;
    }

    std::string file_path = argv[1];
    std::cout<<file_path<<std::endl;
    CC::C0Lexer lexer(file_path);
    CC::Token tocken;
    while ((tocken = lexer.nextToken()).type!= CC::TokenType::END_OF_FILE) {
        std::cout<<tocken.lexeme<<std::endl;
    }

    return 0;
}
