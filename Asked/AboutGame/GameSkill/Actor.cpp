#include "Actor.h"
#include "EpSystem.h"

#include <algorithm>
#include <stdexcept>

namespace Game {
    Actor::Actor() {
        BindEpSystem(EpType::Magic);
    }

    Actor::~Actor() = default;

    bool Actor::UseSkill() {
        if (!epSystem_) {
            throw std::logic_error("EP system is not initialized");
        }
        return epSystem_->UseSkill(*this);
    }

    void Actor::SetEpType(EpType type) {
        epType_ = type;
        BindEpSystem(type);

        maxEp_ = epSystem_->MaxEp();

        furyOverheated_ = false;
        genkiRegenElapsedMs_ = 0;

        if (maxEp_.has_value()) {
            ep_ = std::clamp(ep_, 0, *maxEp_);
        } else {
            ep_ = std::max(ep_, 0);
        }
    }

    void Actor::UpdateLogic(int deltaMs) {
        if (deltaMs <= 0) {
            return;
        }
        if (!epSystem_) {
            throw std::logic_error("EP system is not initialized");
        }
        epSystem_->Update(*this, deltaMs);
    }

    int Actor::GetEp() const {
        return ep_;
    }

    void Actor::SetEp(int value) {
        ep_ = std::max(value, 0);
        if (maxEp_.has_value()) {
            ep_ = std::min(ep_, *maxEp_);
        }
    }

    void Actor::AddEp(int value) {
        if (value <= 0) {
            return;
        }
        SetEp(ep_ + value);
    }

    void Actor::ConsumeEp(int value) {
        if (value <= 0) {
            return;
        }
        ep_ = std::max(0, ep_ - value);
    }

    std::optional<int> Actor::GetMaxEp() const {
        return maxEp_;
    }

    void Actor::SetMaxEp(std::optional<int> value) {
        maxEp_ = value;
        if (maxEp_.has_value()) {
            ep_ = std::min(ep_, *maxEp_);
        }
    }

    bool Actor::IsFuryOverheated() const {
        return furyOverheated_;
    }

    void Actor::SetFuryOverheated(bool value) {
        furyOverheated_ = value;
    }

    int Actor::GetGenkiRegenElapsedMs() const {
        return genkiRegenElapsedMs_;
    }

    void Actor::SetGenkiRegenElapsedMs(int value) {
        genkiRegenElapsedMs_ = std::max(value, 0);
    }

    EpType Actor::GetEpType() const {
        return epType_;
    }

    void Actor::BindEpSystem(EpType type) {
        epSystem_ = CreateEpSystem(type);
        maxEp_ = epSystem_->MaxEp();
    }
} // namespace Game
