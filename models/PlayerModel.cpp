#include "PlayerModel.h"

// J'ai supprimé GROUND_LEVEL qui causait le bug
const float GRAVITY = 0.8f;
const float JUMP_FORCE = -17.f;
const float MOVE_SPEED = 7.0f;
const float DASH_SPEED = 20.0f;

PlayerModel::PlayerModel() {
    // On commence à une position neutre (pas 500 fixe)
    position = sf::Vector2f(100.0f, 2000.0f);
    velocity = sf::Vector2f(0.0f, 0.0f);
    facingDirection = 1;
    state = PlayerState::IDLE;
    dashCooldownTimer = 0.0f;
    attackTimer = 0.0f;
    isDashing = false;
    dashDurationTimer = 0.0f;
}

void PlayerModel::move(float dirX) {
    if (state == PlayerState::ATTACK || isDashing) return;

    velocity.x = dirX * MOVE_SPEED;
    if (dirX > 0) facingDirection = 1;
    if (dirX < 0) facingDirection = -1;

    // Gestion animation course
    if (state != PlayerState::JUMP && state != PlayerState::FALL) {
        state = (dirX != 0) ? PlayerState::RUN : PlayerState::IDLE;
    }
}

void PlayerModel::jump() {
    // CORRECTION : On ne vérifie plus la hauteur (Y), mais l'état.
    // Si on n'est pas déjà en train de sauter ou de tomber, on peut sauter.
    if (state != PlayerState::JUMP && state != PlayerState::FALL && state != PlayerState::ATTACK && !isDashing) {
        velocity.y = JUMP_FORCE;
        state = PlayerState::JUMP;
    }
}

void PlayerModel::attack() {
    if (state != PlayerState::ATTACK && !isDashing) {
        state = PlayerState::ATTACK;
        attackTimer = 0.0f;
        velocity.x = 0;
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

    // --- DASH ---
    if (isDashing) {
        velocity.x = DASH_SPEED * facingDirection;
        velocity.y = 0;
        dashDurationTimer += deltaTime;
        if (dashDurationTimer >= 0.2f) {
            isDashing = false;
            velocity.x = 0;
            // On le met en IDLE par défaut, la frame suivante corrigera si on tombe
            state = PlayerState::IDLE;
        }
    }
    // --- GRAVITÉ ---
    else {
        if(velocity.y<=15){
                velocity.y += GRAVITY;
        }
    }

    position += velocity;

    // --- SAUT vs CHUTE ---
    // On détecte simplement si on monte ou on descend
    if (!isDashing && state != PlayerState::ATTACK) {
        if (velocity.y < 0) state = PlayerState::JUMP;
        else if (velocity.y > 0) state = PlayerState::FALL;
    }

    // --- SUPPRESSION DU BLOC "SOL MAGIQUE" ---
    // C'est ici que j'ai retiré le code qui bloquait le joueur à Y=500.
    // Maintenant, c'est Game.cpp (handleCollisions) qui va l'arrêter
    // quand il touchera une vraie plateforme.

    // --- FIN ATTAQUE ---
    if (state == PlayerState::ATTACK) {
        attackTimer += deltaTime;
        if (attackTimer >= 0.32f) state = PlayerState::IDLE;
    }
}

sf::FloatRect PlayerModel::getHitbox() const {
    // Hitbox plus fine pour être précis
    float width = 30.0f; // J'ai réduit un peu (40->30) pour éviter de flotter sur les bords
    float height = 60.0f;
    return sf::FloatRect(position.x - width/2, position.y - height, width, height);
}
