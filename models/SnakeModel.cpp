#include "SnakeModel.h"
#include <cmath>


/**
 * SnakeModel Constructor
 * Initializes the snake enemy with specific stats: 3 HP, 50.0f speed, and 1 attack damage.
 * Passes EntityType::SNAKE to the base constructor for runtime identification.
 * * @param x The initial X coordinate in the world.
 * @param y The initial Y coordinate in the world.
 */
SnakeModel::SnakeModel(float x, float y)
    : EnemyModel(3, 50.0f, 1, EntityType::SNAKE), // Base character stats
      m_state(SnakeState::WALK),                // Initial state set to walking
      m_stateTimer(0.0f),                       // Timer for state transitions
      m_attackCooldown(0.0f)                    // Initialize attack readiness
{
    m_position = sf::Vector2f(x, y); // Set world position
    m_facingRight = true;            // Inherited orientation flag
}

/**
 * Updates the Snake's AI, physics, and state machine.
 * * @param dt Delta time since the last frame.
 * @param playerPos Current world position of the player (reserved for future AI logic).
 */
void SnakeModel::update(float dt, sf::Vector2f playerPos) {
    if (m_state == SnakeState::DEATH) return;

    // 1. Cooldown
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // 2.  HURT
    if (m_state == SnakeState::HURT) {
        m_stateTimer += dt;
        if (m_stateTimer >= 0.5f) {
            m_state = SnakeState::WALK;
            m_stateTimer = 0;
        }

        m_velocity.x = 30.0f;

        applyGravity(dt);
        m_position += m_velocity * dt;
        return;
    }



    applyGravity(dt);

    updatePatrolMovement(dt);

    m_position += m_velocity * dt;
}

/**
 * Manually changes the current state of the snake.
 * * @param newState The new state (WALK, HURT, DEATH, etc.).
 */
void SnakeModel::setState(SnakeState newState) {
    m_state = newState;
    // Reset timer when entering the HURT state to ensure fixed flinch duration
    if (newState == SnakeState::HURT) {
        m_stateTimer = 0;
    }
}

/**
 * Returns the Axis-Aligned Bounding Box (AABB) for collision detection.
 * Designed for a character with dimensions of 30x30 pixels.
 * * @return sf::FloatRect representing the physical boundaries of the snake.
 */
sf::FloatRect SnakeModel::getHitbox() const {
    // Hitbox: A 30x30 square centered horizontally and positioned upward
    return sf::FloatRect(m_position.x - 15, m_position.y - 30, 30, 30);
}

/**
 * Resets the attack cooldown timer.
 * This is called by the Game class when the snake successfully hits the player.
 */
void SnakeModel::resetAttackCooldown() {
    // Wait for 1 second before the snake is allowed to bite again
    m_attackCooldown = 1.0f;
}
