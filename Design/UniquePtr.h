#pragma once
#include <utility>
#include <iostream>

// 简易 unique ptr
template <class T>
class UniquePtr {
public:
    UniquePtr() noexcept : ptr_(nullptr) {}

    explicit UniquePtr(T *ptr) noexcept : ptr_(ptr) {}

    ~UniquePtr() {
        delete ptr_;
    }

    // 禁止拷贝
    UniquePtr(const UniquePtr &) = delete;

    UniquePtr &operator=(const UniquePtr &) = delete;

    // 允许移动
    UniquePtr(UniquePtr &&other) noexcept
        : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr &operator=(UniquePtr &&other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T *get() const noexcept {
        return ptr_;
    }

    T &operator*() const noexcept {
        return *ptr_;
    }

    T *operator->() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    T *release() noexcept {
        T *temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T *ptr = nullptr) noexcept {
        if (ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }

private:
    T *ptr_;
};

template <class T, class... Args>
UniquePtr<T> MakeUnique(Args &&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
