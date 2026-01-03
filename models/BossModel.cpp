#include "BossModel.h"
#include <cmath>

/**
 * BossModel Constructor
 * Initializes the boss with specific stats: 13 HP, 100.0f speed, and 2 attack damage.
 * Sets the entity type to EntityType::BOSS for runtime identification.
 */
BossModel::BossModel()
    : Character(13, 100.0f, 2, EntityType::BOSS)
{
    // Default world spawn position
    m_position = {2000.0f, 2000.0f};

    // Initial state setup
    m_state = IDLE;
    m_facingRight = false;

    // Initialize internal timers and physics
    m_stateTimer = 0;
    m_attackCooldown = 0;
    m_velocity = sf::Vector2f(0.0f, 0.0f);
}

/**
 * Standard update method
 * Implementation is kept empty as the Boss uses a specific updateBoss method for AI logic.
 */
void BossModel::update(float dt) {
    // Base logic remains empty; AI behavior is handled in updateBoss
}

/**
 * Main AI and Physics update for the Boss
 * Handles gravity, state transitions, movement, and target tracking.
 * @param dt Delta time since last frame.
 * @param playerPos Current world position of the player.
 */
void BossModel::updateBoss(float dt, sf::Vector2f playerPos) {
    // Do nothing if the boss is dead
    if (m_state == DEAD) return;

    // Apply Gravity (constant downward acceleration)
    m_velocity.y += 1500.0f * dt;

    // MOVEMENT LOGIC:
    // The boss stops completely while attacking or being hurt to emphasize the action.
    if (m_state == ATTACKING || m_state == HURT) {
        m_velocity.x = 0;
    } else if (m_state == WALKING) {
        // Move towards the current facing direction
        m_velocity.x = m_facingRight ? m_speed : -m_speed;
    } else {
        // Stop movement if IDLE
        m_velocity.x = 0;
    }

    // Apply calculated velocity to current position
    m_position += m_velocity * dt;

    // Tick down the attack cooldown timer
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // Calculate horizontal distance to the player
    float dist = std::abs(playerPos.x - m_position.x);

    // TARGET TRACKING:
    // Update orientation to face the player, but lock it during ATTACK or HURT states.
    if (dist < 400 && m_state != ATTACKING && m_state != HURT) {
        m_facingRight = (playerPos.x > m_position.x);
    }

    // AI STATE MACHINE
    switch (m_state) {
        case IDLE:
            // Transition to ATTACKING if player is close (within 90 units) and cooldown is over
            if (dist < 90 && m_attackCooldown <= 0) {
                m_state = ATTACKING;
                m_stateTimer = 0;
                m_hasDealtDamage = false; // Reset damage flag for the new attack
            }
            // Transition to WALKING if player is detected within agro range (400 units)
            else if (dist < 400 && dist >90) m_state = WALKING;
            break;

        case WALKING:
            // Transition to ATTACKING if within melee range
            if (dist < 90 && m_attackCooldown <= 0) {
                m_state = ATTACKING;
                m_stateTimer = 0;
                m_hasDealtDamage = false;
            }
            // Stop walking if player moves too far away
            else if (dist >= 400) m_state = IDLE;
            break;

        case ATTACKING:
            m_stateTimer += dt;
            // The attack animation and logic window lasts exactly 0.8 seconds
            if (m_stateTimer > 0.8f) {
                m_state = IDLE;
                m_attackCooldown = 1.2f; // Enforce a 1.2s delay before the next possible attack
            }
            break;

        case HURT:
            m_stateTimer += dt;
            // The recovery period after taking a hit lasts 0.4 seconds
            if (m_stateTimer > 0.4f) m_state = IDLE;
            break;
    }
}

/**
 * Handles taking damage from the player's attacks.
 * @param amount Number of HP to subtract.
 */
void BossModel::takeDamage(int amount) {
    if (m_hp <= 0) return;

    m_hp -= amount;

    if (m_hp <= 0) {
        m_state = DEAD; // Boss enters death state
    } else {
        // Boss flinches (HURT state) on hit
        m_state = HURT;
        m_stateTimer = 0;
    }
}

/**
 * @return The current state of the Boss (IDLE, WALKING, etc.)
 */
BossState BossModel::getState() const { return m_state; }

/**
 * Returns the Axis-Aligned Bounding Box (AABB) for collision detection.
 * The box is roughly 60 units wide and 100 units high, centered horizontally.
 */
sf::FloatRect BossModel::getHitbox() const {
    // Largeur un peu réduite (40 au lieu de 60) pour éviter de coincer dans les murs
    float width = 40.0f;
    float height = 100.0f;

    // CORRECTION ICI : "m_position.y - height"
    // Cela signifie que le rectangle commence 100px au-dessus des pieds
    // et finit exactement aux pieds.
    return sf::FloatRect(m_position.x - width/2, m_position.y - height, width, height);
}

/**
 * Resets the boss state for a level restart or new encounter.
 * @param x The new X spawn coordinate.
 * @param y The new Y spawn coordinate.
 */
void BossModel::reset(float x, float y) {
    setPosition(x, y);
    m_hp = 13;               // Restore full health
    m_state = IDLE;          // Reset behavior state
    m_hasDealtDamage = false; // Reset damage check flag
    m_velocity = sf::Vector2f(0, 0); // Reset physics
}
