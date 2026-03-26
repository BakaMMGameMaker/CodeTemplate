#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 任务类型
enum class ObjectiveType {
    KillEnemy,   // 击杀敌人
    CollectItem, // 收集物品
    TalkToNpc    // 和 NPC 交互
};

// 任务状态
enum class TaskStatus {
    NotAccepted, // 没接收
    Accepted,    // 已接收
    Completed,   // 已完成
    Rewarded     // 已发放奖励
};

// Enum to String
inline string ToString(ObjectiveType type) {
    switch (type) {
        case ObjectiveType::KillEnemy: return "KillEnemy";
        case ObjectiveType::CollectItem: return "CollectItem";
        case ObjectiveType::TalkToNpc: return "TalkToNpc";
        default: return "UnknownObjectiveType";
    }
}

// Enum to String
inline string ToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::NotAccepted: return "NotAccepted";
        case TaskStatus::Accepted: return "Accepted";
        case TaskStatus::Completed: return "Completed";
        case TaskStatus::Rewarded: return "Rewarded";
        default: return "UnknownTaskStatus";
    }
}

// 任务奖励配置
struct RewardConfig {
    int gold = 0; // 给多少金币
    int exp = 0;  // 给多少奖励
};

// 任务目标配置
struct ObjectiveConfig {
    int id = 0; // 配置 id
    ObjectiveType type = ObjectiveType::KillEnemy;
    int targetId = 0;    // 怪物 ID / 物品 ID / NPC ID
    int targetCount = 0; // 需要数量；对话任务 = 1
};

// 静态任务配置
struct TaskConfig {
    int id = 0;                         // 配置 id
    string name;                        // 任务名称
    string desc;                        // 任务描述
    vector<ObjectiveConfig> objectives; // 任务目标
    RewardConfig reward;                // 任务奖励
};

// 动态任务进度
struct ObjectiveProgress {
    int objectiveId = 0;  // 配置 id
    int currentCount = 0; // 当前技术
    bool done = false;    // 是否完成
};

// 动态任务实例
struct TaskInstance {
    int taskId = 0;                              // 任务 id
    TaskStatus status = TaskStatus::NotAccepted; // 任务状态
    vector<ObjectiveProgress> progress;          // 任务进度
};

struct Player {
    int id = 0;   // 玩家 id
    string name;  // 玩家名称
    int gold = 0; // 玩家金币数量
    int exp = 0;  // 玩家经验值

    unordered_map<int, int> inventory; // 玩家背包 itemid => count
};

struct EnemyKilledEvent {
    int enemyId = 0; // 敌人 id
    int count = 1;   // 数量
};

struct ItemCollectedEvent {
    int itemId = 0; // 物品 id
    int count = 1;  // 数量
};

struct TalkToNpcEvent {
    int npcId = 0; // NPC id
};

class TaskSystem {
public:
    explicit TaskSystem(Player &player) : player_(player) {}

    // 添加任务配置
    void AddTaskConfig(const TaskConfig &config) {
        taskConfigs_[config.id] = config;
    }

    // 接收任务
    bool AcceptTask(int taskId) {
        if (!taskConfigs_.count(taskId)) {
            cout << "[AcceptTask] task config not found, taskId = " << taskId << '\n';
            return false;
        }

        // 已经接取过
        if (taskInstances_.count(taskId)) {
            cout << "[AcceptTask] task already exists, taskId = " << taskId << '\n';
            return false;
        }

        // 获取静态配置
        const TaskConfig &cfg = taskConfigs_.at(taskId);

        // 创建任务实例
        TaskInstance instance;
        instance.taskId = taskId;
        instance.status = TaskStatus::Accepted;

        // 初始化任务完成进度
        for (const auto &obj : cfg.objectives) {
            ObjectiveProgress prog;
            prog.objectiveId = obj.id;
            prog.currentCount = 0;
            prog.done = false;
            instance.progress.push_back(prog);
        }

        // 记录任务实例
        taskInstances_[taskId] = instance;

        return true;
    }

    void OnEnemyKilled(const EnemyKilledEvent &e) {
        // 遍历所有任务实例
        for (auto &[taskId, instance] : taskInstances_) {
            if (instance.status != TaskStatus::Accepted) {
                continue;
            }

            // 获取任务静态配置
            const TaskConfig &cfg = taskConfigs_.at(taskId);
            bool changed = false;

            // 遍历任务的所有要求
            for (size_t i = 0; i < cfg.objectives.size(); ++i) {
                // 任务配置
                const ObjectiveConfig &objCfg = cfg.objectives[i];
                // 任务进度
                ObjectiveProgress &prog = instance.progress[i];

                if (prog.done) {
                    continue;
                }

                // 如果要求为杀敌并且击杀的敌人为所需敌人
                if (objCfg.type == ObjectiveType::KillEnemy && objCfg.targetId == e.enemyId) {
                    // 增加技术
                    prog.currentCount += e.count;
                    prog.currentCount = min(prog.currentCount, objCfg.targetCount);
                    // 更新完成状态
                    prog.done = (prog.currentCount >= objCfg.targetCount);
                    changed = true;
                }
            }

            if (changed) {
                RefreshTaskStatus(instance, cfg);
            }
        }
    }

    void OnItemCollected(const ItemCollectedEvent &e) {
        // 更新玩家背包
        player_.inventory[e.itemId] += e.count;

        for (auto &[taskId, instance] : taskInstances_) {
            if (instance.status != TaskStatus::Accepted) {
                continue;
            }

            const TaskConfig &cfg = taskConfigs_.at(taskId);
            bool changed = false;

            for (size_t i = 0; i < cfg.objectives.size(); ++i) {
                const ObjectiveConfig &objCfg = cfg.objectives[i];
                ObjectiveProgress &prog = instance.progress[i];

                if (prog.done) {
                    continue;
                }

                if (objCfg.type == ObjectiveType::CollectItem && objCfg.targetId == e.itemId) {
                    prog.currentCount = min(player_.inventory[e.itemId], objCfg.targetCount);
                    prog.done = (prog.currentCount >= objCfg.targetCount);
                    changed = true;
                }
            }

            if (changed) {
                RefreshTaskStatus(instance, cfg);
            }
        }
    }

    void OnTalkToNpc(const TalkToNpcEvent &e) {
        for (auto &[taskId, instance] : taskInstances_) {
            if (instance.status != TaskStatus::Accepted) {
                continue;
            }

            const TaskConfig &cfg = taskConfigs_.at(taskId);
            bool changed = false;

            for (size_t i = 0; i < cfg.objectives.size(); ++i) {
                const ObjectiveConfig &objCfg = cfg.objectives[i];
                ObjectiveProgress &prog = instance.progress[i];

                if (prog.done) {
                    continue;
                }

                if (objCfg.type == ObjectiveType::TalkToNpc && objCfg.targetId == e.npcId) {
                    prog.currentCount = 1;
                    prog.done = true;
                    changed = true;
                }
            }

            if (changed) {
                RefreshTaskStatus(instance, cfg);
            }
        }
    }

    // 提交任务
    bool SubmitTask(int taskId) {
        // 并非实例
        if (!taskInstances_.count(taskId)) {
            cout << "[SubmitTask] task instance not found, taskId = " << taskId << '\n';
            return false;
        }

        TaskInstance &instance = taskInstances_.at(taskId); // 任务实例
        const TaskConfig &cfg = taskConfigs_.at(taskId);    // 静态配置

        // 还未完成
        if (instance.status != TaskStatus::Completed) {
            cout << "[SubmitTask] task is not completed, taskId = " << taskId << '\n';
            return false;
        }

        // 分发奖励
        player_.gold += cfg.reward.gold;
        player_.exp += cfg.reward.exp;
        instance.status = TaskStatus::Rewarded; // 更新状态

        return true;
    }

private:
    // 更新任务状态
    void RefreshTaskStatus(TaskInstance &instance, const TaskConfig &cfg) {
        bool allDone = true;
        // 是否所有进度都满了
        for (const auto &prog : instance.progress) {
            if (!prog.done) {
                allDone = false;
                break;
            }
        }

        if (allDone) {
            instance.status = TaskStatus::Completed;
        }
    }

    Player &player_;
    unordered_map<int, TaskConfig> taskConfigs_;
    unordered_map<int, TaskInstance> taskInstances_;
};

// {
//     Player player;
//     player.id = 1;
//     player.name = "Nonono";
//
//     TaskSystem taskSystem(player);
//
//     // 任务1：击杀2只狼 + 收集3个草药
//     TaskConfig task1;
//     task1.id = 1001;
//     task1.name = "清理森林";
//     task1.desc = "击杀狼并收集草药";
//     task1.objectives = {
//         {1, ObjectiveType::KillEnemy,   101, 2}, // 杀 2 只 enemyId=101 的怪
//         {2, ObjectiveType::CollectItem, 201, 3}  // 收集 3 个 itemId=201 的物品
//     };
//     task1.reward = {100, 50};
//
//     // 任务2：和村长对话
//     TaskConfig task2;
//     task2.id = 1002;
//     task2.name = "拜访村长";
//     task2.desc = "和村长聊聊";
//     task2.objectives = {
//         {1, ObjectiveType::TalkToNpc, 301, 1}
//     };
//     task2.reward = {30, 20};
//
//     taskSystem.AddTaskConfig(task1);
//     taskSystem.AddTaskConfig(task2);
//
//     taskSystem.AcceptTask(1001);
//     taskSystem.AcceptTask(1002);
//
//     taskSystem.OnEnemyKilled({101, 1});
//     taskSystem.OnItemCollected({201, 1});
//     taskSystem.OnTalkToNpc({301});
//     taskSystem.OnEnemyKilled({101, 1});
//     taskSystem.OnItemCollected({201, 2});
//
//     taskSystem.SubmitTask(1002);
//     taskSystem.SubmitTask(1001);
//
//     return 0;
// }
