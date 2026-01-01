#include "SnakeModel.h"
#include <cmath>

SnakeModel::SnakeModel(float x, float y)
    : Character(3, 50.0f, 1),
      m_state(SnakeState::WALK),
      m_stateTimer(0.0f),
      m_patrolTimer(0.0f),
      m_movingRight(true),
      m_attackCooldown(0.0f)
{
    // MODIFICATION : On utilise les coordonnées données au lieu de 400, 2520
    m_position = sf::Vector2f(x, y);
    m_facingRight = true;
}
void SnakeModel::update(float dt, sf::Vector2f playerPos) {
    if (m_state == SnakeState::DEATH) return;

    // --- 1. Gestion du Cooldown d'Attaque ---
    if (m_attackCooldown > 0) {
        m_attackCooldown -= dt;
    }

    // --- 2. Gestion des Dégâts (HURT) ---
    if (m_state == SnakeState::HURT) {
        m_stateTimer += dt;
        if (m_stateTimer >= 0.5f) {
            m_state = SnakeState::WALK;
            m_stateTimer = 0;
        }
        return; // On ne bouge pas quand on a mal
    }

    // --- 3. Gravité ---
    m_velocity.y += 1500.0f * dt;

    // --- 4. Patrouille (Gauche <-> Droite) ---
    m_patrolTimer += dt;
    if (m_patrolTimer >= 2.0f) { // Change de sens toutes les 2 secondes
        m_movingRight = !m_movingRight;
        m_patrolTimer = 0;
    }

    // Application de la direction
    m_facingRight = m_movingRight;

    if (m_movingRight) {
        m_velocity.x = m_speed;
    } else {
        m_velocity.x = -m_speed;
    }

    // --- 5. Mouvement final ---
    m_position += m_velocity * dt;
}

void SnakeModel::setState(SnakeState newState) {
    m_state = newState;
    if (newState == SnakeState::HURT) {
        m_stateTimer = 0;
    }
}

sf::FloatRect SnakeModel::getHitbox() const {
    // Hitbox : Carré de 30x30 centré
    return sf::FloatRect(m_position.x - 15, m_position.y - 30, 30, 30);
}

void SnakeModel::resetAttackCooldown() {
    m_attackCooldown = 1.0f; // Attend 1 seconde avant de pouvoir remordre
}
