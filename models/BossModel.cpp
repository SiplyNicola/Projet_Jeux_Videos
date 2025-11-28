#include "BossModel.h"
#include <cmath>

BossModel::BossModel() {
    m_position = {800.0f, 500.0f}; // Position de départ
    m_state = IDLE;
    m_hp = 200;
    m_speed = 80.0f;
    m_facingRight = false;
    m_stateTimer = 0;
    m_attackCooldown = 0;
}

void BossModel::update(float dt, sf::Vector2f playerPos) {
    if (m_state == DEAD) return;
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    float dist = std::abs(playerPos.x - m_position.x);

    // Orientation
    if (m_state != ATTACKING && m_state != HURT) {
        m_facingRight = (playerPos.x > m_position.x);
    }

    switch (m_state) {
        case IDLE:
            if (dist < 400) m_state = WALKING;
            break;
        case WALKING:
            if (m_facingRight) m_position.x += m_speed * dt;
            else m_position.x -= m_speed * dt;

            if (dist < 90) { // Portée d'attaque
                if (m_attackCooldown <= 0) {
                    m_state = ATTACKING;
                    m_stateTimer = 0;
                } else m_state = IDLE;
            }
            break;
        case ATTACKING:
            m_stateTimer += dt;
            if (m_stateTimer > 0.8f) { // Fin attaque
                m_state = IDLE;
                m_attackCooldown = 2.0f;
            }
            break;
        case HURT:
            m_stateTimer += dt;
            if (m_stateTimer > 0.4f) m_state = IDLE;
            break;
    }
}

void BossModel::takeDamage(int amount) {
    if (m_state == DEAD) return;
    m_hp -= amount;
    if (m_hp <= 0) m_state = DEAD;
    else {
        m_state = HURT;
        m_stateTimer = 0;
    }
}

sf::Vector2f BossModel::getPosition() const { return m_position; }
BossState BossModel::getState() const { return m_state; }
bool BossModel::isFacingRight() const { return m_facingRight; }
sf::FloatRect BossModel::getHitbox() const {
    return sf::FloatRect(m_position.x - 30, m_position.y - 50, 60, 100);
}
