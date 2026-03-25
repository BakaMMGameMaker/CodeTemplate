#pragma once
#include <typeinfo>
#include <utility>
#include <new>
#include <stdexcept>

class bad_any_cast : public std::bad_cast {
public:
    const char *what() const noexcept override {
        return "bad_any_cast";
    }
};

class any {
    // 操作表基类
    struct ops_base {
        // 析构
        void (*destroy)(any &) noexcept;

        // 拷贝
        void (*copy)(const any &, any &);

        // 移动
        void (*move)(any &, any &) noexcept;

        // 类型信息
        const std::type_info & (*type)() noexcept;

        // 数据指针
        void * (*ptr)(any &) noexcept;

        // 数据指针
        const void * (*cptr)(const any &) noexcept;
    };

    static constexpr std::size_t buffer_size = 3 * sizeof(void *); // 缓冲区大小
    static constexpr std::size_t buffer_align = alignof(void *);   // 缓冲区对齐

    union storage_t {
        void *heap;                                              // 堆区指针
        alignas(buffer_align) unsigned char buffer[buffer_size]; // 缓冲区
    } storage{};

    const ops_base *ops_ = nullptr; // 操作表指针
    bool on_heap_ = false;          // 数据是否位于堆上

    // 类型 <T> 是否应该使用缓冲区
    template <class T>
    static constexpr bool use_small_object =
            sizeof(T) <= buffer_size &&
            alignof(T) <= buffer_align &&
            std::is_nothrow_move_constructible_v<T>;

    // 缓冲区指针
    template <class T>
    static T *small_ptr(any &a) noexcept {
        return reinterpret_cast<T *>(&a.storage.buffer);
    }

    // 缓冲区指针
    template <class T>
    static const T *small_ptr(const any &a) noexcept {
        return reinterpret_cast<const T *>(&a.storage.buffer);
    }

    // 堆区指针
    template <class T>
    static T *heap_ptr(any &a) noexcept {
        return static_cast<T *>(a.storage.heap);
    }

    // 堆区指针
    template <class T>
    static const T *heap_ptr(const any &a) noexcept {
        return static_cast<const T *>(a.storage.heap);
    }

    // 操作表实现
    template <class T>
    struct ops_impl {
        // 析构函数
        static void destroy(any &a) noexcept {
            if constexpr (use_small_object<T>) {
                small_ptr<T>(a)->~T();
            } else {
                delete heap_ptr<T>(a);
            }
            a.ops_ = nullptr;
            a.on_heap_ = false;
        }

        // 拷贝
        static void copy(const any &src, any &dst) {
            if constexpr (use_small_object<T>) {
                new(&dst.storage.buffer) T(*small_ptr<T>(src));
                dst.on_heap_ = false;
            } else {
                dst.storage.heap = new T(*heap_ptr<T>(src));
                dst.on_heap_ = true;
            }
            dst.ops_ = &table;
        }

        // 移动
        static void move(any &src, any &dst) noexcept {
            if constexpr (use_small_object<T>) {
                new(&dst.storage.buffer) T(std::move(*small_ptr<T>(src)));
                small_ptr<T>(src)->~T();
                dst.on_heap_ = false;
            } else {
                dst.storage.heap = src.storage.heap;
                src.storage.heap = nullptr;
                dst.on_heap_ = true;
            }
            dst.ops_ = &table;
            src.ops_ = nullptr;
            src.on_heap_ = false;
        }

        // 类型信息
        static const std::type_info &type() noexcept {
            return typeid(T);
        }

        // 数据指针
        static void *ptr(any &a) noexcept {
            if constexpr (use_small_object<T>) {
                return small_ptr<T>(a);
            } else {
                return heap_ptr<T>(a);
            }
        }

        // 数据指针
        static const void *cptr(const any &a) noexcept {
            if constexpr (use_small_object<T>) {
                return small_ptr<T>(a);
            } else {
                return heap_ptr<T>(a);
            }
        }

        static const ops_base table; // 操作表
    };

public:
    any() noexcept = default;

    any(const any &other) {
        if (other.ops_) {
            other.ops_->copy(other, *this);
        }
    }

    any(any &&other) noexcept {
        if (other.ops_) {
            other.ops_->move(other, *this);
        }
    }

    // 仅 U != any 时启用
    // 1 - 防止 any 里存 any
    // 2 - 防止拷贝和移动被劫持
    template <class T, class U = std::decay_t<T>,
        class = std::enable_if_t<!std::is_same_v<U, any>> >
    any(T &&value) {
        emplace<U>(std::forward<T>(value));
    }

    ~any() {
        reset();
    }

    any &operator=(const any &other) {
        if (this != &other) {
            any tmp(other);
            swap(tmp);
        }
        return *this;
    }

    any &operator=(any &&other) noexcept {
        if (this != &other) {
            reset();
            if (other.ops_) {
                other.ops_->move(other, *this);
            }
        }
        return *this;
    }

    template <class T, class... Args>
    T &emplace(Args &&... args) {
        reset();
        if constexpr (use_small_object<T>) {
            new(&storage.buffer) T(std::forward<Args>(args)...);
            on_heap_ = false;
        } else {
            storage.heap = new T(std::forward<Args>(args)...);
            on_heap_ = true;
        }
        ops_ = &ops_impl<T>::table;
        return *static_cast<T *>(ops_->ptr(*this));
    }

    void reset() noexcept {
        if (ops_) {
            ops_->destroy(*this);
        }
    }

    bool has_value() const noexcept {
        return ops_ != nullptr;
    }

    const std::type_info &type() const noexcept {
        return ops_ ? ops_->type() : typeid(void);
    }

    void swap(any &other) noexcept {
        if (this == &other) return;
        any tmp(std::move(other));
        other = std::move(*this);
        *this = std::move(tmp);
    }

    template <class T>
    friend T *any_cast(any *) noexcept;

    template <class T>
    friend const T *any_cast(const any *) noexcept;
};

template <class T>
const any::ops_base any::ops_impl<T>::table = {
    &any::ops_impl<T>::destroy,
    &any::ops_impl<T>::copy,
    &any::ops_impl<T>::move,
    &any::ops_impl<T>::type,
    &any::ops_impl<T>::ptr,
    &any::ops_impl<T>::cptr
};

template <class T>
T *any_cast(any *a) noexcept {
    if (!a || !a->ops_ || a->type() != typeid(T)) {
        return nullptr;
    }
    return static_cast<T *>(a->ops_->ptr(*a));
}

// any* => T*
template <class T>
const T *any_cast(const any *a) noexcept {
    if (!a || !a->ops_ || a->type() != typeid(T)) {
        return nullptr;
    }
    return static_cast<const T *>(a->ops_->cptr(*a));
}

// any => T
template <class T>
T any_cast(any &a) {
    using U = std::remove_cv_t<std::remove_reference_t<T> >;
    auto p = any_cast<U>(&a);
    if (!p) throw bad_any_cast{};
    return static_cast<T>(*p);
}
