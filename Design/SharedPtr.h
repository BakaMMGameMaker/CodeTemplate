#pragma once
#include <iostream>
#include <cstddef>

// 简易 sharedptr
template <class T>
class SharedPtr {
public:
    SharedPtr() noexcept : ptr_(nullptr), count_(nullptr) {}

    explicit SharedPtr(T *ptr) : ptr_(ptr) {
        if (ptr_) {
            count_ = new std::size_t(1);
        } else {
            count_ = nullptr;
        }
    }

    ~SharedPtr() {
        release();
    }

    // 拷贝构造
    SharedPtr(const SharedPtr &other) noexcept
        : ptr_(other.ptr_), count_(other.count_) {
        if (count_) {
            ++(*count_);
        }
    }

    // 拷贝赋值
    SharedPtr &operator=(const SharedPtr &other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            count_ = other.count_;
            if (count_) {
                ++(*count_);
            }
        }
        return *this;
    }

    // 移动构造
    SharedPtr(SharedPtr &&other) noexcept
        : ptr_(other.ptr_), count_(other.count_) {
        other.ptr_ = nullptr;
        other.count_ = nullptr;
    }

    // 移动赋值
    SharedPtr &operator=(SharedPtr &&other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            count_ = other.count_;
            other.ptr_ = nullptr;
            other.count_ = nullptr;
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

    std::size_t use_count() const noexcept {
        return count_ ? *count_ : 0;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    void reset(T *ptr = nullptr) {
        release();
        if (ptr) {
            ptr_ = ptr;
            count_ = new std::size_t(1);
        }
    }

private:
    void release() noexcept {
        if (count_) {
            --(*count_);
            if (*count_ == 0) {
                delete ptr_;
                delete count_;
            }
        }
        ptr_ = nullptr;
        count_ = nullptr;
    }

    T *ptr_;
    std::size_t *count_;
};
