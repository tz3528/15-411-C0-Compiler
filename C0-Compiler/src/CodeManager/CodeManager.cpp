//
// Created by 陶子杨 on 25-10-23.
//

#include "CodeManager/CodeManager.h"
#include <stdexcept>
#include <iostream>

namespace CC {
    CodeManager::CodeManager(const std::string& file_path) 
        : location{1, 1}, current_pos(0) {
        file_stream.open(file_path);
        if (!file_stream.is_open()) {
            throw std::runtime_error("无法打开文件: " + file_path);
        }
        
        // 读取整个文件到缓冲区
        file_stream.seekg(0, std::ios::end);
        auto file_size = file_stream.tellg();
        file_stream.seekg(0, std::ios::beg);

        buffer.resize(file_size);
        file_stream.read(buffer.data(), file_size);
    }

    Location CodeManager::getLocation() const {
        return location;
    }

    char CodeManager::lookChar() const {
        if (current_pos >= buffer.size()) {
            return '\0';
        }
        return buffer[current_pos];
    }

    char CodeManager::getChar() {
        if (current_pos >= buffer.size()) {
            eof_reached = true;
            return '\0';
        }
        
        char ch = buffer[current_pos++];
        
        if (ch == '\n') {
            location.line++;
            last_column = location.column;
            location.column = 1;
        } else {
            location.column++;
        }
        
        return ch;
    }

    void CodeManager::ungetChar() {
        if (current_pos > 0) {
            current_pos--;
            char ch = buffer[current_pos];
            
            if (ch == '\n') {
                location.line--;
                // 简化处理，实际应该计算上一行的列数
                location.column = last_column;
            } else {
                location.column--;
            }
        }
    }

    bool CodeManager::eofReached() const {
        return eof_reached;
    }

    void CodeManager::updateBuffer(std::vector<char>& new_buffer) {
        buffer = std::move(new_buffer);
    }

    void CodeManager::restar() {
        location = {1, 1};
        current_pos = 0;
        eof_reached = false;
        last_column = 0;
    }
}
