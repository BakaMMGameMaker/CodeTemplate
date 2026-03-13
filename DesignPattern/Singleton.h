#pragma once

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
