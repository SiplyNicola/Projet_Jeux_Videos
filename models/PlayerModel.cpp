#include "PlayerModel.h"
#include <cmath>

// Movement constants defining the physics of the player
const float GRAVITY = 0.8f;      // Downward acceleration applied every frame
const float JUMP_FORCE = -17.f;  // Instant upward velocity applied on jump
const float DASH_SPEED = 20.0f;  // Fixed horizontal velocity during a dash

/**
 * PlayerModel Constructor
 * Initializes the player with specific stats: 5 HP, 7.0f speed, and 1 attack damage.
 * Passes EntityType::PLAYER to the base Character constructor for identification.
 */
PlayerModel::PlayerModel()
    : Character(5, 7.0f, 1, EntityType::PLAYER)
{
    // Initial spawn coordinates for Level 1
    m_position = sf::Vector2f(100.0f, 2520.0f);
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    m_facingRight = true;

    // Initial state and flag setup
    state = PlayerState::IDLE;
    dashCooldownTimer = 0.0f;
    attackTimer = 0.0f;
    isDashing = false;
    dashDurationTimer = 0.0f;
    m_hasDealtDamage = false; // Prevents dealing damage multiple times in one swing
    m_isGrounded = false;     // Tracks if the player is touching a floor tile
    m_attackCooldown = 0.0f;
}

/**
 * Handles horizontal movement input.
 * @param dirX Direction of movement (-1.0f for left, 1.0f for right, 0.0f for none).
 */
void PlayerModel::move(float dirX) {
    // Movement is disabled if the player is currently attacking, dashing, or dead
    if (state == PlayerState::ATTACK || isDashing || state == PlayerState::DEAD) return;

    m_velocity.x = dirX * m_speed;

    // Update orientation based on movement direction
    if (dirX > 0) m_facingRight = true;
    if (dirX < 0) m_facingRight = false;

    // Update state to RUN or IDLE only if not currently in the air
    if (state != PlayerState::JUMP && state != PlayerState::FALL) {
        state = (dirX != 0) ? PlayerState::RUN : PlayerState::IDLE;
    }
}

/**
 * Resurrects the player, resetting health and consuming the extra life.
 */
void PlayerModel::revive() {
    m_hp = 5;                  // Restore initial health points
    state = PlayerState::IDLE; // Reset animation state from DEAD back to IDLE
    m_canRevive = false;       // Mark the one-time revive as used
    m_velocity = sf::Vector2f(0.0f, 0.0f); // Reset physics to prevent sliding on spawn
}

/**
 * Triggers a jump by applying an upward force.
 */
bool PlayerModel::jump() { // <--- Changé void en bool
    // On vérifie si le saut est possible
    if (m_isGrounded && state != PlayerState::DEAD && !isDashing) {
        m_velocity.y = JUMP_FORCE;
        state = PlayerState::JUMP;
        m_isGrounded = false;

        return true; // <--- AJOUT : Le saut a réussi !
    }

    return false; // <--- AJOUT : Le saut a échoué (déjà en l'air, etc.)
}

/**
 * Initiates the attack state.
 */
bool PlayerModel::attack() {
    // Attack cannot be triggered if already attacking, dead, or dashing
    if (m_attackCooldown <= 0 && state != PlayerState::ATTACK && state != PlayerState::DEAD && !isDashing) {
        state = PlayerState::ATTACK;
        attackTimer = 0.0f;
        m_attackCooldown = 1.0f;
        m_velocity.x = 0; // The player stops moving to perform the strike
        m_hasDealtDamage = false; // Reset damage flag for the new attack session

        return true;
    }
    return false;
}

/**
 * Triggers the dash ability if not on cooldown.
 */
bool PlayerModel::dash() {
    if (dashCooldownTimer <= 0 && !isDashing && state != PlayerState::DEAD) {
        isDashing = true;
        dashDurationTimer = 0.0f;
        dashCooldownTimer = 0.5f; // Set a 0.5s delay before the next possible dash
        state = PlayerState::DASH;

        return true;
    }
    return false;
}

/**
 * Handles health reduction and death transition.
 * @param amount Number of HP to subtract.
 */
void PlayerModel::takeDamage(int amount) {
    if (state == PlayerState::DEAD) return;

    m_hp -= amount; // Uses m_hp inherited from the Character base class
    if (m_hp <= 0) {
        m_hp = 0;
        state = PlayerState::DEAD;
        m_velocity.x = 0; // Stop movement on death
    }
}

/**
 * Main logical update loop for the player.
 * Processes physics, timers, and state transitions.
 * @param deltaTime Time elapsed since the last frame.
 */
void PlayerModel::update(float deltaTime) {
    if (state == PlayerState::DEAD) return;

    // Reset grounded status; will be re-validated by collision handling
    m_isGrounded = false;

    // Cooldown management
    if (dashCooldownTimer > 0) dashCooldownTimer -= deltaTime;
    if (m_attackCooldown > 0) m_attackCooldown -= deltaTime;

    // Dash Logic
    if (isDashing) {
        // Dash provides high fixed horizontal velocity in the direction currently faced
        m_velocity.x = DASH_SPEED * (m_facingRight ? 1.0f : -1.0f);
        m_velocity.y = 0; // Gravity is ignored during the dash duration
        dashDurationTimer += deltaTime;

        // Dash ends after 0.2 seconds
        if (dashDurationTimer >= 0.2f) {
            isDashing = false;
            m_velocity.x = 0;
            state = PlayerState::FALL;
        }
    }
    else {
        // Apply Gravity if not dashing
        if(m_velocity.y <= 15) m_velocity.y += GRAVITY;
    }

    // Integrate velocity into position (Inherited from Entity)
    m_position += m_velocity;

    // Detect Vertical State Transitions (Jump/Fall)
    if (!isDashing && state != PlayerState::ATTACK) {
        if (m_velocity.y < -0.1f) state = PlayerState::JUMP;
        else if (m_velocity.y > 0.1f) state = PlayerState::FALL;
    }

    // Attack Animation Timer Management
    if (state == PlayerState::ATTACK) {
        attackTimer += deltaTime;
        // Animation duration is set to 0.32 seconds
        if (attackTimer >= 0.32f) {
            // Return to FALL or IDLE based on vertical movement
            state = (std::abs(m_velocity.y) > 0.1f) ? PlayerState::FALL : PlayerState::IDLE;
        }
    }
}

/**
 * Returns the Axis-Aligned Bounding Box (AABB) for collision detection.
 * Designed for a character with dimensions of 30x60 pixels.
 * @return sf::FloatRect representing the physical boundaries of the player.
 */
sf::FloatRect PlayerModel::getHitbox() const {
    float width = 30.0f;
    float height = 60.0f;
    // The position represents the bottom-center of the character
    return sf::FloatRect(m_position.x - width/2, m_position.y - height, width, height);
}
