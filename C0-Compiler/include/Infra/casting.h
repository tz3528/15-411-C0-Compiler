//
// Created by 陶子杨 on 25-10-27.
//

#pragma once

#include <memory>

namespace INFRA {
    // ---- 检测 To::classof(const From*) 是否存在（编译期约束） ----
    template <typename To, typename From>
    using _classof_expr_t = decltype(To::classof(static_cast<const From*>(nullptr)));

    template <typename To, typename From, typename = void>
    struct _has_classof : std::false_type {};

    template <typename To, typename From>
    struct _has_classof<To, From, std::void_t<_classof_expr_t<To, From>>> : std::true_type {};

    // ---- isa / dyn_cast / cast：面向任意体系（无 RTTI） ----
    template <typename To, typename From>
    inline bool isa(const From* f) {
        static_assert(_has_classof<To, From>::value,
                      "isa<To>(From*): To 必须提供 static bool classof(const From*)");
        return To::classof(f);
    }

    template <typename To, typename From>
    inline bool isa(const From& f) { return isa<To>(&f); }

    template <typename To, typename From>
    inline To* dyn_cast(From* f) {
        return (f && isa<To>(f)) ? static_cast<To*>(f) : nullptr;
    }

    template <typename To, typename From>
    inline const To* dyn_cast(const From* f) {
        return (f && isa<To>(f)) ? static_cast<const To*>(f) : nullptr;
    }

    template <typename To, typename From>
    inline To& cast(From& f) {
        assert(isa<To>(&f) && "cast<To>: 类型不匹配");
        return static_cast<To&>(f);
    }

    template <typename To, typename From>
    inline const To& cast(const From& f) {
        assert(isa<To>(&f) && "cast<To>: 类型不匹配");
        return static_cast<const To&>(f);
    }

    // ---- 智能指针便捷版（可选） ----
    template <typename To, typename From>
    inline bool isa(const std::unique_ptr<From>& p) { return isa<To>(p.get()); }

    template <typename To, typename From>
    inline bool isa(const std::shared_ptr<From>& p) { return isa<To>(p.get()); }

    template <typename To, typename From>
    inline std::shared_ptr<To> dyn_cast(const std::shared_ptr<From>& p) {
        return (p && isa<To>(p)) ? std::static_pointer_cast<To>(p) : std::shared_ptr<To>{};
    }
}
