#include "BossModel.h"
#include <cmath>

BossModel::BossModel() {
    m_position = {800.0f, 100.0f}; // Position de départ
    m_state = IDLE;
    m_hp = 200;
    m_speed = 80.0f;
    m_facingRight = false;
    m_stateTimer = 0;
    m_attackCooldown = 0;
    m_velocity = sf::Vector2f(0.0f, 0.0f);
}

void BossModel::update(float dt, sf::Vector2f playerPos) {
    if (m_state == DEAD) return;

    // 1. APPLIQUER LA GRAVITÉ
    // Utilise la même gravité que le joueur ou un peu plus lourd
    float gravity = 1500.0f;
    m_velocity.y += gravity * dt;

    // 2. MOUVEMENT HORIZONTAL (IA)
    // Au lieu de modifier m_position.x directement, modifie m_velocity.x
    m_velocity.x = 0; // Reset par défaut

    if (m_state == WALKING) {
        if (m_facingRight) m_velocity.x = m_speed;
        else m_velocity.x = -m_speed;
    }

    // 3. APPLIQUER LE MOUVEMENT FINAL
    m_position += m_velocity * dt;

    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    float dist = std::abs(playerPos.x - m_position.x);

    // Orientation
    if (m_state != ATTACKING && m_state != HURT) {
        m_facingRight = (playerPos.x > m_position.x);
    }

    switch (m_state) {
        case IDLE:
            // Cas 1 : Le joueur est déjà à portée de baffes
            if (dist < 90) {
                // Si mon énergie est revenue (Cooldown fini), je tape !
                if (m_attackCooldown <= 0) {
                    m_state = ATTACKING;
                    m_stateTimer = 0;
                }
                // Sinon : Je reste en IDLE le temps que le cooldown descende
            }
            // Cas 2 : Le joueur est un peu loin, je marche vers lui
            else if (dist < 400) {
                m_state = WALKING;
            }
            break;
        case WALKING:
            if (dist < 400){
                if (m_facingRight) m_position.x += m_speed * dt;
                else m_position.x -= m_speed * dt;

                if (dist < 90) { // Portée d'attaque
                    if (m_attackCooldown <= 0) {
                        m_state = ATTACKING;
                        m_stateTimer = 0;
                    } else m_state = IDLE;
                }
            } else m_state = IDLE;
            break;
        case ATTACKING:
            m_stateTimer += dt;
            if (m_stateTimer > 0.8f) { // Fin attaque
                m_state = IDLE;
                m_attackCooldown = 1.0f;
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

void BossModel::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
}

void BossModel::setVelocity(sf::Vector2f v) {
    m_velocity = v;
}

sf::Vector2f BossModel::getVelocity() const {
    return m_velocity;
}

sf::Vector2f BossModel::getPosition() const { return m_position; }
BossState BossModel::getState() const { return m_state; }
bool BossModel::isFacingRight() const { return m_facingRight; }
sf::FloatRect BossModel::getHitbox() const {
    return sf::FloatRect(m_position.x - 30, m_position.y - 50, 60, 100);
}
