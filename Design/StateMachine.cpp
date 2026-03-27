#include <iostream>
#include <memory>

// 简易角色状态机
// 1 - 状态基类，声明接口
// 2 - 角色类，维护指向状态的指针，拥有 ChangeState 函数，调用旧状态 OnExit，新状态 OnEnter
//     角色更新 method：调用所管理的状态的 OnUpdate
// 3 - 具体状态，继承自状态基类，各自实现方法

class Character;

// 状态类
class State {
public:
    virtual ~State() = default;

    // 状态名
    virtual const char *Name() const = 0;

    // 状态进入
    virtual void OnEnter(Character &character) {}
    // 状态更新
    virtual void OnUpdate(Character &character, float dt) {}
    // 状态退出
    virtual void OnExit(Character &character) {}
};

// 角色类
class Character {
public:
    Character();

    void Update(float dt) {
        if (current_state_) {
            current_state_->OnUpdate(*this, dt);
        }
    }

    void ChangeState(std::unique_ptr<State> new_state) {
        if (is_dead_) {
            return;
        }

        if (current_state_) {
            current_state_->OnExit(*this);
        }

        current_state_ = std::move(new_state);

        if (current_state_) {
            current_state_->OnEnter(*this);
        }
    }

    void ForceDead(std::unique_ptr<State> dead_state) {
        if (current_state_) {
            current_state_->OnExit(*this);
        }

        is_dead_ = true;
        current_state_ = std::move(dead_state);

        if (current_state_) {
            current_state_->OnEnter(*this);
        }
    }

    bool IsDead() const {
        return is_dead_;
    }

    void SetMoveInput(bool value) {
        move_input_ = value;
    }

    bool HasMoveInput() const {
        return move_input_;
    }

    void SetAttackInput(bool value) {
        attack_input_ = value;
    }

    bool HasAttackInput() const {
        return attack_input_;
    }

    void ConsumeAttackInput() {
        attack_input_ = false;
    }

private:
    std::unique_ptr<State> current_state_;
    bool is_dead_ = false;

    bool move_input_ = false;
    bool attack_input_ = false;
};

class IdleState;
class MoveState;
class AttackState;
class DeadState;

class IdleState : public State {
public:
    const char *Name() const override {
        return "Idle";
    }

    void OnEnter(Character &) override {}

    void OnUpdate(Character &character, float) override;

    void OnExit(Character &) override {}
};

class MoveState : public State {
public:
    const char *Name() const override {
        return "Move";
    }

    void OnEnter(Character &) override {}

    void OnUpdate(Character &character, float) override;

    void OnExit(Character &) override {}
};

class AttackState : public State {
public:
    const char *Name() const override {
        return "Attack";
    }

    void OnEnter(Character &) override {
        attack_timer_ = 0.0f;
    }

    void OnUpdate(Character &character, float dt) override;

    void OnExit(Character &) override {}

private:
    float attack_timer_ = 0.0f;
};

class DeadState : public State {
public:
    const char *Name() const override {
        return "Dead";
    }

    void OnEnter(Character &) override {}

    void OnUpdate(Character &, float) override {}

    void OnExit(Character &) override {}
};

Character::Character() {
    ChangeState(std::make_unique<IdleState>());
}

void IdleState::OnUpdate(Character &character, float) {
    if (character.HasAttackInput()) {
        character.ConsumeAttackInput();
        character.ChangeState(std::make_unique<AttackState>());
        return;
    }

    if (character.HasMoveInput()) {
        character.ChangeState(std::make_unique<MoveState>());
        return;
    }
}

void MoveState::OnUpdate(Character &character, float) {
    if (character.HasAttackInput()) {
        character.ConsumeAttackInput();
        character.ChangeState(std::make_unique<AttackState>());
        return;
    }

    if (!character.HasMoveInput()) {
        character.ChangeState(std::make_unique<IdleState>());
        return;
    }
}

void AttackState::OnUpdate(Character &character, float dt) {
    attack_timer_ += dt;

    if (attack_timer_ >= 0.8f) {
        character.ChangeState(std::make_unique<IdleState>());
    }
}

int main() {
    Character hero;

    std::cout << "\n--- 开始移动 ---\n";
    hero.SetMoveInput(true);
    hero.Update(0.1f);
    hero.Update(0.1f);

    std::cout << "\n--- 发起攻击 ---\n";
    hero.SetAttackInput(true);
    hero.Update(0.1f);
    hero.Update(0.3f);
    hero.Update(0.3f);
    hero.Update(0.3f);

    std::cout << "\n--- 停止移动 ---\n";
    hero.SetMoveInput(false);
    hero.Update(0.1f);

    std::cout << "\n--- 死亡 ---\n";
    hero.ForceDead(std::make_unique<DeadState>());
    hero.Update(0.1f);

    std::cout << "\n--- 尝试再次切换 ---\n";
    hero.ChangeState(std::make_unique<IdleState>());

    return 0;
}
