#ifndef SNAKEMODEL_H
#define SNAKEMODEL_H

#include "Character.h"
#include "Entity.h"

enum class SnakeState {
    WALK,
    HURT,
    DEATH
};

class SnakeModel : public Character {
public:
    SnakeModel();

    void update(float dt) override {} // Vide pour satisfaire le parent

    // Fonction principale de mise à jour
    void update(float dt, sf::Vector2f playerPos);

    SnakeState getState() const { return m_state; }
    void setState(SnakeState newState);

    sf::FloatRect getHitbox() const;

    // --- GESTION DU DÉLAI D'ATTAQUE ---
    bool canAttack() const { return m_attackCooldown <= 0; } // Vrai si le timer est fini
    void resetAttackCooldown(); // Le serpent doit attendre avant de remordre
    // ----------------------------------

private:
    SnakeState m_state;
    float m_stateTimer;   // Pour gérer la durée du clignotement rouge

    // Pour la patrouille
    float m_patrolTimer;  // Chronomètre pour changer de direction
    bool m_movingRight;   // Dans quel sens on va ?

    // Pour l'attaque
    float m_attackCooldown; // Temps d'attente entre deux morsures
};

#endif
