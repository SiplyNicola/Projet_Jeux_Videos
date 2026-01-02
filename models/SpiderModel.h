#ifndef SPIDERMODEL_H
#define SPIDERMODEL_H

#include "Character.h"
#include "EnemyModel.h"

enum class SpiderState {
    HANGING,    // Au plafond (En attente)
    DROPPING,   // En chute libre
    WALK,       // Au sol (Comportement Serpent)
    ATTACK,     // Au sol (Morsure)
    HURT,
    DEATH
};

class SpiderModel : public EnemyModel {
public:
    SpiderModel(float x, float y);

    void update(float dt) override {}
    void update(float dt, sf::Vector2f playerPos);

    SpiderState getState() const { return m_state; }
    void setState(SpiderState newState);
    sf::FloatRect getHitbox() const override;

    // Pour dessiner le fil quand elle est suspendue
    float getAnchorY() const { return m_anchorY; }
    bool isHanging() const { return m_state == SpiderState::HANGING || m_state == SpiderState::DROPPING; }

    // Gestion attaque
    bool canAttack() const { return m_attackCooldown <= 0; }
    void resetAttackCooldown();

private:
    SpiderState m_state;
    float m_stateTimer;
    float m_anchorY;      // Le plafond
    float m_attackCooldown;

};

#endif
