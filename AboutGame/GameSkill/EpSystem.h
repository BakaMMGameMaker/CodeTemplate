#pragma once

#include <memory>
#include <optional>

namespace Game {
    class Actor;

    class IEpSystem {
    public:
        virtual ~IEpSystem() = default;

        virtual bool UseSkill(Actor &actor) = 0;

        virtual void Update(Actor &actor, int deltaMs) = 0;

        virtual std::optional<int> MaxEp() const = 0;
    };

    std::unique_ptr<IEpSystem> CreateEpSystem(class EpType type);
} // namespace Game
