#include "EnemyModel.h"

// Constructor: Initializes default patrol variables
EnemyModel::EnemyModel(int hp, float speed, int damage, EntityType type)
    : Character(hp, speed, damage, type),
      m_patrolTimer(0.0f),
      m_patrolDuration(2.0f), // By default, the enemy changes direction every 2 seconds
      m_movingRight(true)
{
    // We can initialize other common variables here if needed
}

// Centralized gravity implementation
void EnemyModel::applyGravity(float dt) {
    // 1500.0f is an arbitrary gravity constant that "feels" right in the game.
    // Applying it here prevents accidental gravity discrepancies between a Rat and a Snake.
    m_velocity.y += 1500.0f * dt;
}

// Centralized patrol AI implementation
void EnemyModel::updatePatrolMovement(float dt) {
    // 1. Update Timer
    m_patrolTimer += dt;

    // 2. Check threshold (Change Direction)
    if (m_patrolTimer >= m_patrolDuration) {
        m_movingRight = !m_movingRight; // Invert boolean
        m_patrolTimer = 0.0f;           // Reset timer
    }

    // 3. Visual and physical application
    m_facingRight = m_movingRight; // Updates sprite orientation (Character)

    // If moving right, positive speed. Otherwise, negative.
    m_velocity.x = m_movingRight ? m_speed : -m_speed;
}
