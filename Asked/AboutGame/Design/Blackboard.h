#pragma once
#include <memory>
#include <stdexcept>
#include <unordered_map>

// 1 - 非模板类 BaseHolder - 外表一致的集装箱，只有虚析构和一个纯虚克隆函数

// 2 - 模板类 Holder - 集装箱内存储的各种类型的数据
// --- 要实现的方法
// --- 1 - 模板单参数构造函数，在 Value 构造只能指针时使用
// --- 2 - 重写父类的克隆操作

// 3 - 非模板类 Value - 存储指向具体集装箱的指针及集装箱内数据的类型信息
// --- 要实现的方法
// --- 1 - 模板函数 TypeTag，返回指定类型的身份标签，本质是一个地址，用于判定请求的类型与实际存储的类型是否一致
// --- 2 - 模板单参数构造函数，被黑板的 emplace 方法调用，调用对应类型的 Holder 的单参数构造函数，并记录类型信息
// --- 3 - 模板函数 is，判断请求的类型与实际存储的类型是否一致
// --- 4 - 模板函数 get，用于从所指向的集装箱中获取实际存储的数据，使用 is 方法来判断获取是否合法

// 4 - 非模板类 Blackboard - 对 unordered_map<string, Value> 的包装
// --- 要实现的方法
// --- 1 - 模板函数 emplace，先构造指定类型的对象，再用对象构造 Value 容器，最后存放到表中
// --- 2 - 模板函数 get，先判断表中是否有请求的 key，再直接返回所存 Value 的 get 方法的返回值

// 从需求入手，判断上层框架需要什么接口
// 1 - unordered_map 的值类型是固定的，所以需要一个非模板 Value 类
// 2 - emplace 需要从类型 T 的临时对象构造出一个 Value 对象，所以 Value 需要一个模板构造函数
// 3 - 由于 Value 本身并非模板类，所以无法存储实际值，只能存储一个指向实际值的指针，因此需要模板类 Holder
// 4 - 又因指针类型也无法含有模板参数，所以需要一个非模板 BaseHolder，而 Holder 则继承自 BaseHolder
// 5 - 这就决定了 Value 需要存储指向 BaseHolder 的指针，指针指向特定类型的 Holder，Holder 内存储实际数据
// 6 - 同时 Value 内还需要一个字段来记录 BaseHolder 中存储的究竟是什么类型的数据
// 7 - get 方法需要从 Value 中获取实际数据，所以 Value 需要一个 get 方法
// 8 - Value 中的 get 方法需要做：1 - 判断给定类型是否和实际存储的数据类型匹配 2 - 返回存储的实际数据或指向数据的指针

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
