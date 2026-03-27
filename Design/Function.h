#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>
#include <stdexcept>

// std function 简易实现

template <typename>
class Function;

template <typename R, typename... Args>
class Function<R(Args...)> {
    // 对外统一类型
    struct Base {
        virtual ~Base() = default;

        virtual R invoke(Args &&... args) = 0;

        // 注意，克隆返回的是一个管理自己克隆体的智能指针，不是直接返回值
        virtual std::unique_ptr<Base> clone() const = 0;
    };

    // 持有实际函数
    template <typename F>
    struct Holder : Base {
        F func;

        template <typename U>
        explicit Holder(U &&f) : func(std::forward<U>(f)) {}

        R invoke(Args &&... args) override {
            return func(std::forward<Args>(args)...);
        }

        std::unique_ptr<Base> clone() const override {
            return std::make_unique<Holder<F> >(func);
        }
    };

    // 并不知道实际类型，所以必须指向非模板 Base
    std::unique_ptr<Base> ptr_;

public:
    Function() = default;

    explicit Function(std::nullptr_t) {}

    // 用任意可调用对象构造
    template <typename F>
    // requires (!std::is_same_v<std::decay_t<F>, Function>)
    explicit Function(F &&f) {
        using Decayed = std::decay_t<F>;
        ptr_ = std::make_unique<Holder<Decayed> >(std::forward<F>(f));
    }

    // 拷贝构造
    Function(const Function &other) {
        if (other.ptr_) {
            ptr_ = other.ptr_->clone();
        }
    }

    // 移动构造
    Function(Function &&other) noexcept = default;

    // 拷贝赋值
    Function &operator=(const Function &other) {
        if (this == &other) return *this;
        ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;
        return *this;
    }

    // 移动赋值
    Function &operator=(Function &&other) noexcept = default;

    // 置空
    Function &operator=(std::nullptr_t) {
        ptr_.reset();
        return *this;
    }

    // 调用
    R operator()(Args... args) const {
        if (!ptr_) {
            throw std::runtime_error("bad function call");
        }
        return ptr_->invoke(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept {
        return static_cast<bool>(ptr_);
    }
};
