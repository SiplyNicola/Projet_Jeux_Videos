#include "BossModel.h"
#include <cmath>

BossModel::BossModel() : Entity(13, 100.0f, 2) {
    m_position = {2000.0f, 2000.0f};
    m_state = IDLE;
    m_facingRight = false;
    m_stateTimer = 0;
    m_attackCooldown = 0;
    m_velocity = sf::Vector2f(0.0f, 0.0f);
}

void BossModel::update(float dt) {
    // Logique de base ou vide, l'IA utilise updateBoss
}

void BossModel::updateBoss(float dt, sf::Vector2f playerPos) {
    if (m_state == DEAD) return;

    m_velocity.y += 1500.0f * dt; // Gravité

    // MODIF : On annule la vitesse X si on attaque (le boss s'arrête pour frapper)
    if (m_state == ATTACKING || m_state == HURT) {
        m_velocity.x = 0;
    } else if (m_state == WALKING) {
        m_velocity.x = m_facingRight ? m_speed : -m_speed;
    } else {
        m_velocity.x = 0;
    }

    m_position += m_velocity * dt;

    if (m_attackCooldown > 0) m_attackCooldown -= dt;
    float dist = std::abs(playerPos.x - m_position.x);

    // Orientation verrouillée pendant l'attaque
    if (dist < 400 && m_state != ATTACKING && m_state != HURT) {
        m_facingRight = (playerPos.x > m_position.x);
    }

    switch (m_state) {
        case IDLE:
            if (dist < 90 && m_attackCooldown <= 0) {
                m_state = ATTACKING;
                m_stateTimer = 0;
                m_hasDealtDamage = false;
            } else if (dist < 400) m_state = WALKING;
            break;

        case WALKING:
            if (dist < 90 && m_attackCooldown <= 0) {
                m_state = ATTACKING;
                m_stateTimer = 0;
                m_hasDealtDamage = false;
            } else if (dist >= 400) m_state = IDLE;
            break;

        case ATTACKING:
            m_stateTimer += dt;
            // L'attaque dure 0.8s au total
            if (m_stateTimer > 0.8f) {
                m_state = IDLE;
                m_attackCooldown = 1.2f; // Délai avant la prochaine attaque
            }
            break;

        case HURT:
            m_stateTimer += dt;
            if (m_stateTimer > 0.4f) m_state = IDLE;
            break;
    }
}

void BossModel::takeDamage(int amount) {
    if (m_hp <= 0) return;
    m_hp -= amount;
    if (m_hp <= 0) m_state = DEAD;
    else {
        m_state = HURT;
        m_stateTimer = 0;
    }
}

BossState BossModel::getState() const { return m_state; }

sf::FloatRect BossModel::getHitbox() const {
    return sf::FloatRect(m_position.x - 30, m_position.y - 50, 60, 100);
}
