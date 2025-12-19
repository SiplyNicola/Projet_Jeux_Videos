#include "PlayerModel.h"

// J'ai supprimé GROUND_LEVEL qui causait le bug
const float GRAVITY = 0.8f;
const float JUMP_FORCE = -17.f;
const float DASH_SPEED = 20.0f;

PlayerModel::PlayerModel() : Character(5, 7.0f, 1) {
    // On commence à une position neutre (pas 500 fixe)
    m_position = sf::Vector2f(100.0f, 2520.0f); // On utilise m_position de Entity
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    m_facingRight = true;
    state = PlayerState::IDLE;
    dashCooldownTimer = 0.0f;
    attackTimer = 0.0f;
    isDashing = false;
    dashDurationTimer = 0.0f;
}

void PlayerModel::move(float dirX) {
    if (state == PlayerState::ATTACK || isDashing) return;

    m_velocity.x = dirX * m_speed;
    if (dirX > 0) m_facingRight = true;
    if (dirX < 0) m_facingRight = false;

    // Gestion animation course
    if (state != PlayerState::JUMP && state != PlayerState::FALL) {
        state = (dirX != 0) ? PlayerState::RUN : PlayerState::IDLE;
    }
}

void PlayerModel::jump() {
    // CORRECTION : On ne vérifie plus la hauteur (Y), mais l'état.
    // Si on n'est pas déjà en train de sauter ou de tomber, on peut sauter.
    if (std::abs(m_velocity.y) < 0.1f && state != PlayerState::ATTACK && !isDashing) {
        m_velocity.y = JUMP_FORCE;
        state = PlayerState::JUMP;
    }
}

void PlayerModel::attack() {
    if (state != PlayerState::ATTACK && !isDashing) {
        state = PlayerState::ATTACK;
        attackTimer = 0.0f;
        m_velocity.x = 0;
        m_hasDealtDamage = false;
    }
}

void PlayerModel::dash() {
    if (dashCooldownTimer <= 0 && !isDashing && state != PlayerState::ATTACK) {
        isDashing = true;
        dashDurationTimer = 0.0f;
        dashCooldownTimer = 0.5f;
        state = PlayerState::DASH;
    }
}

void PlayerModel::update(float deltaTime) {
    if (dashCooldownTimer > 0) dashCooldownTimer -= deltaTime;

    if (isDashing) {
        // Utilise m_facingRight pour la direction du dash
        m_velocity.x = DASH_SPEED * (m_facingRight ? 1.0f : -1.0f);
        m_velocity.y = 0;
        dashDurationTimer += deltaTime;
        if (dashDurationTimer >= 0.2f) {
            isDashing = false;
            m_velocity.x = 0;
            state = PlayerState::FALL;
            m_velocity.y = GRAVITY;
        }
    }
    else {
        if(m_velocity.y <= 15) m_velocity.y += GRAVITY;
    }

    m_position += m_velocity;

    if (!isDashing && state != PlayerState::ATTACK) {
        if (m_velocity.y < 0) state = PlayerState::JUMP;
        else if (m_velocity.y > 0) state = PlayerState::FALL;
    }

    if (state == PlayerState::ATTACK) {
        attackTimer += deltaTime;
        if (attackTimer >= 0.32f) {
            if (std::abs(m_velocity.y) > 0.1f) state = PlayerState::FALL;
            else state = PlayerState::IDLE;
        }
    }
}

void PlayerModel::takeDamage(int amount) {
    if (state == PlayerState::DEAD) return;

    m_hp -= amount;
    if (m_hp <= 0) {
        m_hp = 0;
        state = PlayerState::DEAD;
        m_velocity.x = 0; // On l'arrête
    }
}

sf::FloatRect PlayerModel::getHitbox() const {
    float width = 30.0f;
    float height = 60.0f;
    return sf::FloatRect(m_position.x - width/2, m_position.y - height, width, height);
}
