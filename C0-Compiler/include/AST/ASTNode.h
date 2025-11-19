#pragma once

#include "Lexer/Lexer.h"
#include <memory>

namespace CC {

    template <typename Derived>
    class ASTNode {
    public:
        Location location{};
        
        ASTNode() = default;

        template <typename Visitor>
        auto accept(Visitor& visitor) -> decltype(visitor.visit(std::declval<Derived&>()));
    private:
        // 记录节点所包含的token范围
        int min_range = 0, max_range = 0;
    };
    
    // 定义智能指针类型
    template<typename T>
    using ASTNodePtr = std::shared_ptr<T>;
}