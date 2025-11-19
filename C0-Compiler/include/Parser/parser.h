//
// Created by 陶子杨 on 25-10-27.
//

#pragma once

namespace CC {
    template <typename Derived>
    class Parser {
    public:
        void parse() {
            static_cast<Derived*>(this)->parse();
        }
    protected:
        int position = 0;
    };
}
