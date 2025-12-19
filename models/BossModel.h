#ifndef BOSS_MODEL_H
#define BOSS_MODEL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Character.h"

enum BossState { IDLE, WALKING, ATTACKING, HURT, DEAD };

class BossModel : public Character {
public:
    BossModel();

    bool m_hasDealtDamage;

    void updateBoss(float dt, sf::Vector2f playerPos);
    void update(float dt) override;

    void takeDamage(int amount) override;
    sf::FloatRect getHitbox() const override;
    BossState getState() const;
    float getStateTimer() const { return m_stateTimer; }


private:
    BossState m_state;
    float m_stateTimer;
    float m_attackCooldown;

};
#endif
