#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <functional>

// 简易对象池
template <class T>
class ObjectPool {
public:
    // 重置函数
    using ResetFn = std::function<void(T &)>;

    explicit ObjectPool(std::size_t capacity, ResetFn reset_fn = {})
        : in_use_(capacity, false),
          reset_fn_(std::move(reset_fn)) {
        objects_.reserve(capacity);
        free_indices_.reserve(capacity);

        // 创建 capacity 个对象
        for (std::size_t i = 0; i < capacity; ++i) {
            objects_.push_back(std::make_unique<T>());
            free_indices_.push_back(i);
        }
    }

    // 获取一个空闲对象
    T *Acquire() {
        if (free_indices_.empty()) {
            return nullptr;
        }

        std::size_t idx = free_indices_.back();
        free_indices_.pop_back();
        in_use_[idx] = true;
        return objects_[idx].get();
    }

    // 回收给定的对象
    void Release(T *obj) {
        if (obj == nullptr) {
            return;
        }

        // 不属于此池
        std::size_t idx = IndexOf(obj);
        if (idx == npos) {
            std::cerr << "Release error: object does not belong to this pool\n";
            return;
        }

        // 已经被回收
        if (!in_use_[idx]) {
            std::cerr << "Release error: object already released\n";
            return;
        }

        // 重置对象
        if (reset_fn_) {
            reset_fn_(*obj);
        }

        in_use_[idx] = false;
        free_indices_.push_back(idx);
    }

    [[nodiscard]] std::size_t Capacity() const {
        return objects_.size();
    }

    [[nodiscard]] std::size_t Available() const {
        return free_indices_.size();
    }

    [[nodiscard]] std::size_t InUse() const {
        return Capacity() - Available();
    }

private:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

    std::size_t IndexOf(T *ptr) const {
        for (std::size_t i = 0; i < objects_.size(); ++i) {
            if (objects_[i].get() == ptr) {
                return i;
            }
        }
        return npos;
    }

    std::vector<std::unique_ptr<T> > objects_; // 指向每个对象的指针
    std::vector<std::size_t> free_indices_;    // 空闲的对象的索引
    std::vector<bool> in_use_;                 // 每个对象是否正在被使用
    ResetFn reset_fn_;                         // 对象重置函数
};
