#ifndef SNAKEMODEL_H
#define SNAKEMODEL_H

#include "Character.h"
#include "Entity.h"
#include "EnemyModel.h"

enum class SnakeState {
    WALK,
    HURT,
    DEATH
};

class SnakeModel : public EnemyModel {
public:
    SnakeModel(float x, float y);

    void update(float dt) override {}


    void update(float dt, sf::Vector2f playerPos);

    SnakeState getState() const { return m_state; }
    void setState(SnakeState newState);

    sf::FloatRect getHitbox() const;


    bool canAttack() const { return m_attackCooldown <= 0; }
    void resetAttackCooldown();


private:
    SnakeState m_state;
    float m_stateTimer;

    float m_attackCooldown;
};

#endif
