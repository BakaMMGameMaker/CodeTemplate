#include "EpSystem.h"
#include "Actor.h"

#include <memory>
#include <stdexcept>

namespace Game {
    namespace {
        constexpr int kMagicMaxEp = 10;
        constexpr int kMagicSkillCost = 2;
        constexpr int kMagicRegenIntervalMs = 1000;
        constexpr int kMagicRegenAmount = 1;

        constexpr int kFuryMaxEp = 5;
        constexpr int kFuryTickIntervalMs = 1000;

        constexpr int kGenkiSkillCost = 10;
        constexpr int kGenkiRegenIntervalMs = 2000;
        constexpr int kGenkiRegenAmount = 5;

        class MagicEpSystem final : public IEpSystem {
        public:
            bool UseSkill(Actor &actor) override {
                if (actor.GetEp() < kMagicSkillCost) {
                    return false;
                }
                actor.ConsumeEp(kMagicSkillCost);
                return true;
            }

            void Update(Actor &actor, int deltaMs) override {
                tickElapsedMs_ += deltaMs;
                const int tickCount = tickElapsedMs_ / kMagicRegenIntervalMs;
                tickElapsedMs_ %= kMagicRegenIntervalMs;

                for (int i = 0; i < tickCount; ++i) {
                    actor.AddEp(kMagicRegenAmount);
                }
            }

            std::optional<int> MaxEp() const override {
                return kMagicMaxEp;
            }

        private:
            int tickElapsedMs_ = 0;
        };

        class FuryEpSystem final : public IEpSystem {
        public:
            bool UseSkill(Actor &actor) override {
                if (!actor.IsFuryOverheated() && actor.GetEp() < kFuryMaxEp) {
                    actor.AddEp(1);
                }
                return true;
            }

            void Update(Actor &actor, int deltaMs) override {
                tickElapsedMs_ += deltaMs;
                const int tickCount = tickElapsedMs_ / kFuryTickIntervalMs;
                tickElapsedMs_ %= kFuryTickIntervalMs;

                for (int i = 0; i < tickCount; ++i) {
                    Tick(actor);
                }
            }

            std::optional<int> MaxEp() const override {
                return kFuryMaxEp;
            }

        private:
            static void Tick(Actor &actor) {
                if (actor.IsFuryOverheated()) {
                    actor.ConsumeEp(1);
                    if (actor.GetEp() == 0) {
                        actor.SetFuryOverheated(false);
                    }
                    return;
                }

                if (actor.GetEp() >= kFuryMaxEp) {
                    actor.SetFuryOverheated(true);
                }
            }

            int tickElapsedMs_ = 0;
        };

        class GenkiEpSystem final : public IEpSystem {
        public:
            bool UseSkill(Actor &actor) override {
                if (actor.GetEp() >= kGenkiSkillCost) {
                    actor.ConsumeEp(kGenkiSkillCost);
                }
                return true;
            }

            void Update(Actor &actor, int deltaMs) override {
                int elapsed = actor.GetGenkiRegenElapsedMs() + deltaMs;
                const int tickCount = elapsed / kGenkiRegenIntervalMs;
                elapsed %= kGenkiRegenIntervalMs;
                actor.SetGenkiRegenElapsedMs(elapsed);

                if (tickCount > 0) {
                    actor.AddEp(tickCount * kGenkiRegenAmount);
                }
            }

            std::optional<int> MaxEp() const override {
                return std::nullopt;
            }
        };
    } // namespace

    std::unique_ptr<IEpSystem> CreateEpSystem(EpType type) {
        switch (type) {
            case EpType::Magic: return std::make_unique<MagicEpSystem>();
            case EpType::Fury: return std::make_unique<FuryEpSystem>();
            case EpType::Genki: return std::make_unique<GenkiEpSystem>();
        }
        throw std::invalid_argument("Unknown EpType");
    }
} // namespace Game
