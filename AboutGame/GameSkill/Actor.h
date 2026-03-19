#pragma once

#include <memory>
#include <optional>

namespace Game {
    // 技能类别
    enum class EpType {
        Magic, // 魔法
        Fury,  // 怒气
        Genki, // 元气
    };

    class IEpSystem;

    // 角色类
    class Actor {
    public:
        Actor();

        ~Actor();

        // 释放技能
        bool UseSkill();

        // 设置技能类别
        void SetEpType(EpType type);

        // 更新状态
        void UpdateLogic(int deltaMs);

        // 获取能量
        int GetEp() const;

        // 设置能量
        void SetEp(int value);

        // 增添能量
        void AddEp(int value);

        // 消费能量
        void ConsumeEp(int value);

        // 能量上限，返回 optional，因为有的技能没有能量上限
        std::optional<int> GetMaxEp() const;

        // 设置能量上限
        void SetMaxEp(std::optional<int> value);

        // 怒气技能正处于冷却状态
        bool IsFuryOverheated() const;

        // 设置怒气技能的过载状态
        void SetFuryOverheated(bool value);

        // 获取元气技能增添能量经过的时间
        int GetGenkiRegenElapsedMs() const;

        // 设置元气技能增添能量经过的时间
        void SetGenkiRegenElapsedMs(int value);

        // 获取技能类型
        EpType GetEpType() const;

    private:
        // 绑定到技能系统
        void BindEpSystem(EpType type);

        int ep_ = 0;                  // 当前能量
        std::optional<int> maxEp_;    // 能量上限
        bool furyOverheated_ = false; // 怒气技能过载
        int genkiRegenElapsedMs_ = 0; // 元气技能经过时间

        EpType epType_ = EpType::Magic;       // 技能类别
        std::unique_ptr<IEpSystem> epSystem_; // 技能系统
    };
} // namespace Game
