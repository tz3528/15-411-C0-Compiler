//
// Created by 陶子杨 on 25-10-23.
//

#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

namespace CC {

    struct Location {
        size_t line, column;
    };

    class CodeManager {
    public:
        explicit CodeManager(const std::string& file_path);

        Location location;

        Location getLocation() const;

        char lookChar() const;
        char getChar();
        void ungetChar();

        bool eofReached() const;

        void updateBuffer(std::vector<char>& new_buffer);

        void restar();

    protected:
        std::ifstream file_stream;

        std::vector<char> buffer;
        bool eof_reached = false;
        size_t current_pos = 0;
        size_t last_column = 0;
    };
}
