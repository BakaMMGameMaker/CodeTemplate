#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <memory>
#include <algorithm>
#include <string>

// 简易 EventBus

// Event Base Class
struct IEvent {
    virtual ~IEvent() = default;
};

// 事件总线
class EventBus {
public:
    using ListenerId = std::size_t;

private:
    struct ICallbackList {
        virtual ~ICallbackList() = default;
    };

    template <typename EventType>
    struct CallbackList : ICallbackList {
        struct Listener {
            ListenerId id;                                   // 订阅者是谁
            std::function<void(const EventType &)> callback; // 订阅者提供了什么回调
        };

        std::vector<Listener> listeners; // 订阅者列表
    };

public:
    EventBus() : nextListenerId_(1) {}

    // subscribe<事件类型>(具体回调) 回调不返回值，参数为要监听的事件类型
    template <typename EventType>
    ListenerId subscribe(std::function<void(const EventType &)> callback) {
        // typeid - 获取一个类型或者对象的运行时类型信息，返回 const std::type_info&
        // type_index - 对 type_info 的轻量封装，允许进行比较，放到 map 里
        auto type = std::type_index(typeid(EventType));

        // 如果此 type 先前不存在，建立 type => CallbackList 映射
        if (callbackMap_.find(type) == callbackMap_.end()) {
            callbackMap_[type] = std::make_unique<CallbackList<EventType> >();
        }

        // callbackMap_[type].get() = ICallbackList*
        // type 和 EventType 对应，因此可以 Cast 成 CallbackList<EventType>
        auto *list = static_cast<CallbackList<EventType> *>(callbackMap_[type].get());

        ListenerId id = nextListenerId_++;
        // 添加订阅者
        list->listeners.push_back({id, std::move(callback)});
        return id; // 返回订阅句柄，这里是简单订阅者 id，后期可以做成 Hanlder 等方便取消订阅
    }

    template <typename EventType>
    void unsubscribe(ListenerId id) {
        auto type = std::type_index(typeid(EventType));
        auto it = callbackMap_.find(type);
        // 事件类型不存在
        if (it == callbackMap_.end()) {
            return;
        }

        // it->second.get() = ICallbackList*
        auto *list = static_cast<CallbackList<EventType> *>(it->second.get());
        auto &listeners = list->listeners;

        // 在订阅列表中删除订阅者
        listeners.erase(
            std::remove_if(
                listeners.begin(),
                listeners.end(),
                [id](const typename CallbackList<EventType>::Listener &listener) {
                    return listener.id == id;
                }
            ),
            listeners.end()
        );
    }

    // 给发布者发布消息
    template <typename EventType>
    void publish(const EventType &event) {
        // 发布者发布的是什么类型的消息
        auto type = std::type_index(typeid(EventType));
        auto it = callbackMap_.find(type);
        // 当前没有订阅者
        if (it == callbackMap_.end()) {
            return;
        }

        // 获取订阅此类型事件的所有订阅者
        auto *list = static_cast<CallbackList<EventType> *>(it->second.get());

        // 避免回调过程中修改监听列表，做一个浅拷贝
        auto listenersCopy = list->listeners; // listeners = vector<Listener>, Listener = id + callback
        for (const auto &listener : listenersCopy) {
            if (listener.callback) {
                listener.callback(event); // 调用回调，并传入给定的 event
            }
        }
    }

private:
    // type_index 代表了事件类型，unique_ptr<ICallbackList> 管理所有订阅此类型事件的订阅者
    // 注意，如果直接存 CallbackList，会导致需要很多的 Map 来对应不同的类型事件
    // 因此统一存放 ICallbackList，并在后期 Cast 成对应类型的 CallbackList
    std::unordered_map<std::type_index, std::unique_ptr<ICallbackList> > callbackMap_;
    ListenerId nextListenerId_;
};

struct PlayerDamagedEvent : public IEvent {
    int playerId;
    int damage;
};

struct EnemyKilledEvent : public IEvent {
    int enemyId;
    std::string enemyName;
};

// 示例使用
/*
    EventBus bus;

    auto damageListenerId = bus.subscribe<PlayerDamagedEvent>(
        [](const PlayerDamagedEvent& e)
        {
            std::cout << "[PlayerDamagedEvent] playerId="
                      << e.playerId
                      << ", damage="
                      << e.damage
                      << std::endl;
        }
    );

    auto killListenerId = bus.subscribe<EnemyKilledEvent>(
        [](const EnemyKilledEvent& e)
        {
            std::cout << "[EnemyKilledEvent] enemyId="
                      << e.enemyId
                      << ", enemyName="
                      << e.enemyName
                      << std::endl;
        }
    );

    bus.publish(PlayerDamagedEvent{1, 25});
    bus.publish(EnemyKilledEvent{1001, "Goblin"});

    bus.unsubscribe<PlayerDamagedEvent>(damageListenerId);

    std::cout << "---- after unsubscribe ----" << std::endl;
    bus.publish(PlayerDamagedEvent{1, 50});   // 不再触发
    bus.publish(EnemyKilledEvent{1002, "Orc"});

*/
