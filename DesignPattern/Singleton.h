#pragma once
#include <mutex>

// 单例模式模板

// C++11 后的稳定写法
// 1 - 静态 Get 方法, 返回引用, 避免判空
// 2 - 私有构造函数与析构函数, 禁用拷贝构造与赋值
// - static instance 只会初始化一次

/* - C++11 起局部静态变量初始化是线程安全的
 * -- 在 C++11 之前这点难以保证, 导致多线程情况下很难确定变量的状态, 如初始化多次, 读到半初始化状态等, 导致锁滥用
 * -- C++11 之后添加了这个标准, 单例模式从中受益
 *
 * - 编译器的实现细节
 * -- 给每一个局部静态变量配备一个 guard, 第一次初始化的时候占用锁或者标记原子判定,
 * -- 构造成功后标记 guard, 失败或抛异常则不标记成功, 以便下次重试
 * -- 保证只有一个线程真正执行初始化, 其它的线程要读则等待, 要初始化则跳过
 */

// - 不需要手动 new 和手动释放, 程序结束后自动析构
class Singleton {
public :
    static Singleton &Get() {
        static Singleton instance; // 静态存储区, 不在栈上
        return instance;
    }

    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;

private:
    Singleton() = default;

    ~Singleton() = default; // 析构也要私有，不然外面可以手动调用析构函数导致双重析构
};

// 饿汉式
// 类加载时就创建对象
// 优点 - 无线程安全问题, 缺点 - 程序启用就创建, 可能导致资源浪费
class Singleton2 {
public:
    static Singleton2 &Get() { return instance; }

    Singleton2(const Singleton2 &) = delete;

    Singleton2 &operator=(const Singleton2 &) = delete;

private:
    Singleton2() = default;

    ~Singleton2() = default;

    static Singleton2 instance;
};

Singleton2 Singleton2::instance;

// 懒汉式 - 第一次用时才创建
// 线程不安全, 多个线程读取到 instance 为 nullptr 时会创建多个对象, 尽量不要用
class Singleton3 {
public:
    static Singleton3 *Get() {
        if (not instance) instance = new Singleton3();
        return instance;
    }

    Singleton3(const Singleton3 &) = delete;

    Singleton3 &operator=(const Singleton3 &) = delete;

private:
    Singleton3() = default;

    ~Singleton3() = default;

    static Singleton3 *instance;
};

Singleton3 *Singleton3::instance = nullptr;

// 懒汉带锁
// - 每次 Get 都要加锁，性能低下
class Singleton3Lock {
public:
    static Singleton3Lock *Get() {
        std::lock_guard lock(mtx);
        if (not instance) instance = new Singleton3Lock();
        return instance;
    }

    Singleton3Lock(const Singleton3Lock &) = delete;

    Singleton3Lock &operator=(const Singleton3Lock &) = delete;

private:
    Singleton3Lock() = default;

    ~Singleton3Lock() = default;

    static Singleton3Lock *instance;
    static std::mutex mtx;
};

Singleton3Lock *Singleton3Lock::instance = nullptr;
std::mutex Singleton3Lock::mtx;

// 双检锁 DCL Double Check Locking
// 在 CPU 重排序下依然危险的原因
// 即便 instance 非空, 由于 CPU 指令重排的问题, 没有保证实例对象已经彻底构造完毕
// 比如先分配内存, 然后直接赋值给 instance, 再调用构造函数
// 这样就导致了线程可能会读取到半成品对象, 因此仍然不安全
class SingletonDCL {
public:
    static SingletonDCL *Get() {
        if (not instance) { // 避免每次都加锁
            std::lock_guard lock(mtx);
            // 这里可能进来两个线程, 所以要再判断一次
            // A 过第一重检查 拿锁 创建实例
            // B 过第一重检查 拿锁 发现 instance 非空 返回
            if (not instance) instance = new SingletonDCL();
        }
        return instance;
    }

    SingletonDCL(const SingletonDCL &) = delete;

    SingletonDCL &operator=(const SingletonDCL &) = delete;

private:
    SingletonDCL() = default;

    ~SingletonDCL() = default;

    static SingletonDCL *instance;
    static std::mutex mtx;
};

SingletonDCL *SingletonDCL::instance = nullptr;
std::mutex SingletonDCL::mtx;
