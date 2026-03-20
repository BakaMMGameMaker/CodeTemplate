#pragma once
#include <memory>
#include <stdexcept>
#include <unordered_map>

// 需要有 BaseHolder 的原因: 面向 unordered_map 仅体现出唯一一种类型
// 给所有的 Holder<T> 都提供一种父类型，让他们都可以转成 BaseHolder*
struct BaseHolder {
    virtual ~BaseHolder() = default;

    virtual std::unique_ptr<BaseHolder> clone() const = 0;
};

// 存放具体值，真正装载数据
template <typename T>
struct Holder : BaseHolder {
    T value;

    template <typename U>
    explicit Holder(U &&v) : value(std::forward<U>(v)) {}

    // 克隆自己
    std::unique_ptr<BaseHolder> clone() const override {
        return std::make_unique<Holder<T> >(value);
    }
};

// 对外提供存和取的接口，作为 Blackboard 里真正存的值类型
// 本质上是持有一个 BaseHolder 指针，指针指向某个具体的 Holder<T>
class Value {
    std::unique_ptr<BaseHolder> ptr_; // 指向‘数据盒子’的指针。‘盒子’指 BaseHolder，数据为 Holder<T>
    const void *type_ = nullptr;      // 记录 ptr_ 存放的具体类型(身份码)。为 TypeTag 返回值

    template <typename T> // 必须有，这样不同类型才有不同身份码
    static const void *TypeTag() {
        static int tag;
        return &tag; // 用唯一的地址代表各种类型标签
    }

public:
    Value() = default;

    Value(const Value &other) : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr),
                                type_(other.type_) {}

    Value &operator=(const Value &other) {
        if (this == &other) return *this;
        ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;
        type_ = other.type_;
        return *this;
    }

    Value(Value &&) noexcept = default;

    Value &operator=(Value &&) noexcept = default;

    // 在 emplace 中由数据类型 T 构造 Value 对象时调用
    template <typename T>
    Value(T &&v) {
        // 规范化类型，const int&, int&& ... => int
        using U = std::decay_t<T>;
        // 分配空间并创建指向资源的指针
        ptr_ = std::make_unique<Holder<U> >(std::forward<T>(v));
        // 补充类型信息，T 一致的数据会拥有一样的标签；标签本质上是一个地址
        type_ = TypeTag<U>();
    }

    // Value 中存储的数据的类别是否和 T 一致
    template <typename T>
    bool is() const { return type_ == TypeTag<std::decay_t<T> >(); }

    // 从 Value 中获取类型为 T 的数据
    // 如果类型不匹配，throw
    template <typename T>
    T &get() {
        using U = std::decay_t<T>;
        if (!is<U>()) throw std::bad_cast();
        return static_cast<Holder<U> *>(ptr_.get())->value;
    }

    template <typename T>
    const T &get() const {
        using U = std::decay_t<T>;
        if (!is<U>()) throw std::bad_cast();
        return static_cast<const Holder<U> *>(ptr_.get())->value;
    }

    bool empty() const { return ptr_ == nullptr; }
};

class Blackboard {
    std::unordered_map<std::string, Value> mp_;

public:
    Blackboard() = default;

    // T: 打算插入的数据类型，如 int，string
    // Args: 构造数据所用到的参数
    template <typename T, typename... Args>
    void emplace(const std::string &key, Args &&... args) {
        // 1 - 用完美转发构造实际的 T 类型对象
        // 2 - 用临时对象构造 Value 对象
        // Value 包含指向实际数据的指针 ptr_ 与一个代表类型 T 的类型信息 type_
        mp_.emplace(key, Value(T(std::forward<Args>(args)...)));
    }

    template <typename T>
    T &get(const std::string &key) {
        // 1 - 有没有这个 key
        auto it = mp_.find(key);
        if (it == mp_.end()) throw std::out_of_range("key not found");
        // 2 - 这个 key 对应的数据类型究竟是不是 T (在 get 中检测)
        return it->second.get<T>();
    }

    template <typename T>
    const T &get(const std::string &key) const {
        auto it = mp_.find(key);
        if (it == mp_.end()) throw std::out_of_range("key not found");
        return it->second.get<T>();
    }

    bool contains(const std::string &key) const {
        return mp_.find(key) != mp_.end();
    }
};
