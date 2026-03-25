#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

// 允许 dynamic cast 的版本，比起原版会更加简单
// 1 - Value 中不需要 type_ 字段和 TypeTag 方法

struct BaseHolder {
    virtual ~BaseHolder() = default;

    virtual std::unique_ptr<BaseHolder> clone() const = 0;
};

template <typename T>
struct Holder : BaseHolder {
    T value;

    template <typename U>
    explicit Holder(U &&v) : value(std::forward<U>(v)) {}

    std::unique_ptr<BaseHolder> clone() const override {
        return std::make_unique<Holder<T> >(value);
    }
};

class Value {
    std::unique_ptr<BaseHolder> ptr_;

public:
    Value() = default;

    Value(const Value &other) : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}

    Value &operator=(const Value &other) {
        if (this == &other) return *this;
        ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;
        return *this;
    }

    Value(Value &&) noexcept = default;

    Value &operator=(Value &&) noexcept = default;

    template <typename T>
    Value(T &&v) {
        using U = std::decay_t<T>;
        ptr_ = std::make_unique<Holder<U> >(std::forward<T>(v));
    }

    template <typename T>
    bool is() const {
        using U = std::decay_t<T>;
        return dynamic_cast<const Holder<U> *>(ptr_.get()) != nullptr;
    }

    template <typename T>
    T &get() {
        using U = std::decay_t<T>;
        auto *p = dynamic_cast<Holder<U> *>(ptr_.get());
        if (!p) throw std::bad_cast();
        return p->value;
    }

    template <typename T>
    const T &get() const {
        using U = std::decay_t<T>;
        auto *p = dynamic_cast<const Holder<U> *>(ptr_.get());
        if (!p) throw std::bad_cast();
        return p->value;
    }

    bool empty() const { return ptr_ == nullptr; }
};

class Blackboard {
    std::unordered_map<std::string, Value> mp_;

public:
    Blackboard() = default;

    template <typename T, typename... Args>
    void emplace(const std::string &key, Args &&... args) {
        mp_.emplace(key, Value(T(std::forward<Args>(args)...)));
    }

    template <typename T>
    T &get(const std::string &key) {
        auto it = mp_.find(key);
        if (it == mp_.end()) throw std::out_of_range("key not found");
        return it->second.get<T>();
    }

    template <typename T>
    const T &get(const std::string &key) const {
        auto it = mp_.find(key);
        if (it == mp_.end()) throw std::out_of_range("key not found");
        return it->second.get<T>();
    }

    bool contains(const std::string &key) const { return mp_.find(key) != mp_.end(); }
};
