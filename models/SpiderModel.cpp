#include "SpiderModel.h"
#include <cmath>

/**
 * SpiderModel Constructor
 * Initializes the spider with specific stats: 3 HP, 50.0f speed, and 1 attack damage.
 * Passes EntityType::SPIDER to the base constructor for runtime identification.
 * * @param x The initial X coordinate in the world.
 * @param y The initial Y coordinate, which also serves as the ceiling anchor.
 */
SpiderModel::SpiderModel(float x, float y)
    : Character(3, 50.0f, 1, EntityType::SPIDER),
      m_state(SpiderState::HANGING),    // Starts in ambush mode, hanging from the ceiling
      m_stateTimer(0.0f),               // Timer for state durations (like being hurt)
      m_anchorY(y),                     // CRITICAL: This Y-coordinate fixes the ceiling position for the thread
      m_attackCooldown(0.0f),            // Delay between consecutive attacks
      m_patrolTimer(0.0f)
{
    m_position = sf::Vector2f(x, y); // Initial world position
    m_facingRight = true;            // Inherited orientation flag
}

/**
 * Updates the Spider's AI, physics, and state transitions.
 * @param dt Delta time since the last frame.
 * @param playerPos Current world position of the player used for detection.
 */
void SpiderModel::update(float dt, sf::Vector2f playerPos) {
    // Stop all logic if the entity is dead
    if (m_state == SpiderState::DEATH) return;

    // 1. Attack Cooldown Management
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // 2. Damage Recovery (HURT state)
    // Triggers a brief period of immobility after being hit
    if (m_state == SpiderState::HURT) {
        m_stateTimer += dt;
        if (m_stateTimer >= 0.5f) {
            // Once recovery is over, resume normal walking behavior
            m_state = SpiderState::WALK;
            m_stateTimer = 0;
        }
        return; // Skip movement logic while hurt
    }

    // 3. State Machine Logic
    switch (m_state) {

    // --- PHASE 1: THE AMBUSH (Ceiling Trap) ---
    case SpiderState::HANGING:
        m_velocity = sf::Vector2f(0.f, 0.f); // Remains immobile on the ceiling

        // PLAYER DETECTION LOGIC
        // If the player is within 150 pixels horizontally and below the spider...
        if (std::abs(playerPos.x - m_position.x) < 150.0f &&
            playerPos.y > m_position.y &&                    // Player is below
            playerPos.y < m_position.y + 600.0f) {           // Player is not too deep

            // Trigger the ambush
            m_state = SpiderState::DROPPING;
        }
        break;

    case SpiderState::DROPPING:
        // Free-fall physics logic
        m_velocity.x = 0;
        m_velocity.y += 2000.0f * dt; // High gravity for a fast drop
        m_position += m_velocity * dt;

        // NOTE: Transition to WALK state is handled in Game::resolveEnemyCollision
        // upon detecting contact with the ground!
        break;

    // --- PHASE 2: ACTIVE PATROL (Ground Phase) ---
    case SpiderState::WALK:
    case SpiderState::ATTACK:
        // Standard gravity while on the ground
        m_velocity.y += 1500.0f * dt;

        // Basic Patrol Behavior (Left <-> Right)
        // Switches direction every 2 seconds
        m_patrolTimer += dt;
        if (m_patrolTimer >= 2.0f) {
            m_movingRight = !m_movingRight;
            m_patrolTimer = 0;
        }

        m_facingRight = m_movingRight;
        m_velocity.x = m_movingRight ? m_speed : -m_speed;

        // Apply final velocity to position
        m_position += m_velocity * dt;
        break;
    }
}

/**
 * Manually changes the spider's behavior state.
 * @param newState The new state to apply.
 */
void SpiderModel::setState(SpiderState newState) {
    m_state = newState;
    // Ensure timer is reset if entering the hurt state to guarantee recovery time
    if (newState == SpiderState::HURT) m_stateTimer = 0;
}

/**
 * Returns the Axis-Aligned Bounding Box (AABB) for collision detection.
 * @return sf::FloatRect representing the 30x30 hitbox.
 */
sf::FloatRect SpiderModel::getHitbox() const {
    return sf::FloatRect(m_position.x - 15, m_position.y - 30, 30, 30);
}

/**
 * Resets the attack cooldown timer.
 * Prevents the spider from dealing damage every frame upon contact.
 */
void SpiderModel::resetAttackCooldown() {
    m_attackCooldown = 1.0f; // 1-second delay before next bite
}
