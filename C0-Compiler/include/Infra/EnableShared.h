//
// Created by 陶子杨 on 25-11-11.
//

#pragma once

#include <memory>

namespace INFRA {

    template <typename T>
    class EnableShared: public std::enable_shared_from_this<T> {
    public:
        std::shared_ptr<T> createShared() {
            return std::enable_shared_from_this<EnableShared>::shared_from_this();
        }
    };
}
