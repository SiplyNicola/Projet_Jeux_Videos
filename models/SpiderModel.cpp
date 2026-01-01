#include "SpiderModel.h"
#include <cmath>

SpiderModel::SpiderModel(float x, float y)
    : Character(40, 120.0f, 20), // Un peu plus forte que le serpent
      m_state(SpiderState::HANGING),
      m_stateTimer(0.0f),
      m_anchorY(y),
      m_attackCooldown(0.0f),
      m_patrolTimer(0.0f),
      m_movingRight(true)
{
    m_position = sf::Vector2f(x, y);
    m_facingRight = true;
}

void SpiderModel::update(float dt, sf::Vector2f playerPos) {
    if (m_state == SpiderState::DEATH) return;

    // Gestion du cooldown d'attaque
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // Gestion du HURT (Recul/Clignotement)
    if (m_state == SpiderState::HURT) {
        m_stateTimer += dt;
        if (m_stateTimer >= 0.5f) {
            // Si on était au sol, on retourne marcher, sinon on tombe
            m_state = SpiderState::WALK;
            m_stateTimer = 0;
        }
        return;
    }

    switch (m_state) {
    // --- PHASE 1 : LE PIÈGE ---
    case SpiderState::HANGING:
        m_velocity = sf::Vector2f(0.f, 0.f); // Immobile

        // DÉTECTION DU JOUEUR
        // Si le joueur est proche en X (horizontal) et en dessous
        if (std::abs(playerPos.x - m_position.x) < 150.0f && // 150 pixels de large
            playerPos.y > m_position.y &&                    // Joueur en dessous
            playerPos.y < m_position.y + 600.0f) {           // Pas trop bas non plus

            m_state = SpiderState::DROPPING;
        }
        break;

    case SpiderState::DROPPING:
        // Gravité seulement (elle tombe)
        m_velocity.x = 0;
        m_velocity.y += 2000.0f * dt; // Gravité forte
        m_position += m_velocity * dt;
        // Le passage à l'état WALK se fera dans Game.cpp lors de la collision avec le sol !
        break;

    // --- PHASE 2 : COMME LE SERPENT ---
    case SpiderState::WALK:
    case SpiderState::ATTACK:
        // Gravité normale
        m_velocity.y += 1500.0f * dt;

        // Patrouille (Gauche <-> Droite)
        m_patrolTimer += dt;
        if (m_patrolTimer >= 2.0f) {
            m_movingRight = !m_movingRight;
            m_patrolTimer = 0;
        }

        m_facingRight = m_movingRight;
        m_velocity.x = m_movingRight ? m_speed : -m_speed;

        m_position += m_velocity * dt;
        break;
    }
}

void SpiderModel::setState(SpiderState newState) {
    m_state = newState;
    if (newState == SpiderState::HURT) m_stateTimer = 0;
}

sf::FloatRect SpiderModel::getHitbox() const {
    return sf::FloatRect(m_position.x - 15, m_position.y - 30, 30, 30);
}

void SpiderModel::resetAttackCooldown() {
    m_attackCooldown = 1.0f;
}
