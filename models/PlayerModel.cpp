#include "PlayerModel.h"
#include <cmath>

// Constantes de mouvement
const float GRAVITY = 0.8f;
const float JUMP_FORCE = -17.f;
const float DASH_SPEED = 20.0f;

PlayerModel::PlayerModel() : Character(5, 7.0f, 1) { // 5 PV, 7.0 vitesse, 1 dégât
    m_position = sf::Vector2f(100.0f, 2520.0f); // Position initiale au spawn
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    m_facingRight = true;

    state = PlayerState::IDLE;
    dashCooldownTimer = 0.0f;
    attackTimer = 0.0f;
    isDashing = false;
    dashDurationTimer = 0.0f;
    m_hasDealtDamage = false;
}

void PlayerModel::move(float dirX) {
    // On ne peut pas marcher si on attaque ou si on dash
    if (state == PlayerState::ATTACK || isDashing || state == PlayerState::DEAD) return;

    m_velocity.x = dirX * m_speed;
    if (dirX > 0) m_facingRight = true;
    if (dirX < 0) m_facingRight = false;

    // Mise à jour de l'état au sol
    if (state != PlayerState::JUMP && state != PlayerState::FALL) {
        state = (dirX != 0) ? PlayerState::RUN : PlayerState::IDLE;
    }
}

void PlayerModel::jump() {
    // On ne peut sauter que si on touche le sol (vitesse verticale proche de 0)
    if (std::abs(m_velocity.y) < 0.1f && state != PlayerState::DEAD && !isDashing) {
        m_velocity.y = JUMP_FORCE;
        state = PlayerState::JUMP;
    }
}

void PlayerModel::attack() {
    if (state != PlayerState::ATTACK && state != PlayerState::DEAD && !isDashing) {
        state = PlayerState::ATTACK;
        attackTimer = 0.0f;
        m_velocity.x = 0; // On s'arrête pour frapper
        m_hasDealtDamage = false;
    }
}

void PlayerModel::dash() {
    if (dashCooldownTimer <= 0 && !isDashing && state != PlayerState::DEAD) {
        isDashing = true;
        dashDurationTimer = 0.0f;
        dashCooldownTimer = 0.5f; // Délai avant le prochain dash
        state = PlayerState::DASH;
    }
}

void PlayerModel::takeDamage(int amount) {
    if (state == PlayerState::DEAD) return;

    m_hp -= amount; // Utilise m_hp de Character
    if (m_hp <= 0) {
        m_hp = 0;
        state = PlayerState::DEAD;
        m_velocity.x = 0;
    }
}

void PlayerModel::update(float deltaTime) {
    if (state == PlayerState::DEAD) return;

    if (dashCooldownTimer > 0) dashCooldownTimer -= deltaTime;

    if (isDashing) {
        m_velocity.x = DASH_SPEED * (m_facingRight ? 1.0f : -1.0f);
        m_velocity.y = 0; // Pas de gravité pendant le dash
        dashDurationTimer += deltaTime;

        if (dashDurationTimer >= 0.2f) { // Durée du dash
            isDashing = false;
            m_velocity.x = 0;
            state = PlayerState::FALL;
        }
    }
    else {
        // Application de la gravité
        if(m_velocity.y <= 15) m_velocity.y += GRAVITY;
    }

    // Application du mouvement à la position héritée d'Entity
    m_position += m_velocity;

    // Détermination de l'état JUMP/FALL si on n'est pas en train d'attaquer
    if (!isDashing && state != PlayerState::ATTACK) {
        if (m_velocity.y < -0.1f) state = PlayerState::JUMP;
        else if (m_velocity.y > 0.1f) state = PlayerState::FALL;
    }

    // Gestion du timer d'attaque pour revenir à l'état IDLE
    if (state == PlayerState::ATTACK) {
        attackTimer += deltaTime;
        if (attackTimer >= 0.32f) { // Durée de l'animation d'attaque
            state = (std::abs(m_velocity.y) > 0.1f) ? PlayerState::FALL : PlayerState::IDLE;
        }
    }
}

sf::FloatRect PlayerModel::getHitbox() const {
    // Hitbox ajustée pour un ninja de 30x60 pixels
    float width = 30.0f;
    float height = 60.0f;
    return sf::FloatRect(m_position.x - width/2, m_position.y - height, width, height);
}
