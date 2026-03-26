#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

struct Vec2 {
    float x = 0.f;
    float y = 0.f;
};

static float DistanceSq(const Vec2 &a, const Vec2 &b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// 角色简易实现
class Actor {
public:
    Actor(std::string name, int hp, int mp, Vec2 pos)
        : name_(std::move(name)), hp_(hp), mp_(mp), pos_(pos) {}

    const std::string &GetName() const { return name_; }
    int GetHp() const { return hp_; }
    int GetMp() const { return mp_; }
    const Vec2 &GetPosition() const { return pos_; }

    bool IsAlive() const { return hp_ > 0; }

    void ConsumeMp(int value) {
        mp_ = std::max(0, mp_ - value);
    }

    void RecoverHp(int value) {
        hp_ += value;
    }

    void TakeDamage(int value) {
        hp_ = std::max(0, hp_ - value);
    }

    void PrintStatus() const {
        std::cout << name_ << " | HP: " << hp_ << " | MP: " << mp_ << "\n";
    }

private:
    std::string name_;
    int hp_ = 100;
    int mp_ = 100;
    Vec2 pos_;
};

// 技能配置
struct SkillConfig {
    int id = 0;            // 技能 id
    std::string name;      // 技能名称
    float cooldown = 0.f;  // CD
    int manaCost = 0;      // 耗蓝
    float castRange = 0.f; // 范围
};

// 技能上下文
struct CastContext {
    Actor *caster = nullptr; // 发射者
    Actor *target = nullptr; // 目标
    Vec2 targetPos{};
};

// 技能效果接口
class ISkillEffect {
public:
    virtual ~ISkillEffect() = default;

    // 应用效果
    virtual void Execute(const CastContext &ctx) = 0;
};

// 具体效果
class DamageEffect : public ISkillEffect {
public:
    explicit DamageEffect(int damage) : damage_(damage) {}

    void Execute(const CastContext &ctx) override {
        if (!ctx.target) {
            std::cout << "DamageEffect failed: no target.\n";
            return;
        }
        if (!ctx.target->IsAlive()) {
            std::cout << ctx.target->GetName() << " is already dead.\n";
            return;
        }

        ctx.target->TakeDamage(damage_);
    }

private:
    int damage_ = 0;
};

class HealEffect : public ISkillEffect {
public:
    explicit HealEffect(int heal) : heal_(heal) {}

    void Execute(const CastContext &ctx) override {
        if (!ctx.target) {
            std::cout << "HealEffect failed: no target.\n";
            return;
        }
        if (!ctx.target->IsAlive()) {
            std::cout << "Heal failed: " << ctx.target->GetName() << " is dead.\n";
            return;
        }

        ctx.target->RecoverHp(heal_);
    }

private:
    int heal_ = 0;
};

// 技能类
class Skill {
public:
    explicit Skill(SkillConfig config)
        : config_(std::move(config)) {}

    int GetId() const { return config_.id; }
    const std::string &GetName() const { return config_.name; }

    // 增添技能具有的效果
    void AddEffect(std::unique_ptr<ISkillEffect> effect) {
        effects_.push_back(std::move(effect));
    }

    // 更新技能
    void Update(float dt) {
        if (cooldownRemaining_ > 0.f) {
            cooldownRemaining_ -= dt;
            if (cooldownRemaining_ < 0.f) {
                cooldownRemaining_ = 0.f;
            }
        }
    }

    // 是否允许释放
    bool CanCast(const CastContext &ctx) const {
        if (!ctx.caster) {
            std::cout << "Cast failed: no caster.\n";
            return false;
        }

        if (!ctx.caster->IsAlive()) {
            std::cout << "Cast failed: caster is dead.\n";
            return false;
        }

        if (cooldownRemaining_ > 0.f) {
            std::cout << config_.name << " is on cooldown: "
                    << cooldownRemaining_ << "s remaining.\n";
            return false;
        }

        if (ctx.caster->GetMp() < config_.manaCost) {
            std::cout << "Cast failed: not enough MP.\n";
            return false;
        }

        if (ctx.target) {
            float distSq = DistanceSq(ctx.caster->GetPosition(), ctx.target->GetPosition());
            if (distSq > config_.castRange * config_.castRange) {
                std::cout << "Cast failed: target out of range.\n";
                return false;
            }
        }

        return true;
    }

    // 释放技能
    bool Cast(const CastContext &ctx) {
        if (!CanCast(ctx)) {
            return false;
        }

        ctx.caster->ConsumeMp(config_.manaCost);
        cooldownRemaining_ = config_.cooldown;

        for (auto &effect : effects_) {
            effect->Execute(ctx);
        }

        return true;
    }

private:
    SkillConfig config_;
    float cooldownRemaining_ = 0.f;
    std::vector<std::unique_ptr<ISkillEffect> > effects_;
};

// 技能组件：角色持有多个技能
class SkillComponent {
public:
    explicit SkillComponent(Actor *owner) : owner_(owner) {}

    // 添加技能
    void AddSkill(std::unique_ptr<Skill> skill) {
        int id = skill->GetId();
        skills_[id] = std::move(skill);
    }

    // 更新组件
    void Update(float dt) {
        for (auto &[id, skill] : skills_) {
            skill->Update(dt);
        }
    }

    // 释放技能
    bool CastSkill(int skillId, Actor *target) {
        auto it = skills_.find(skillId);
        if (it == skills_.end()) {
            std::cout << "Skill not found: " << skillId << "\n";
            return false;
        }

        CastContext ctx;
        ctx.caster = owner_;
        ctx.target = target;
        if (target) {
            ctx.targetPos = target->GetPosition();
        }

        return it->second->Cast(ctx);
    }

private:
    Actor *owner_ = nullptr;
    std::unordered_map<int, std::unique_ptr<Skill> > skills_;
};
