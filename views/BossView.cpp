#include "BossView.h"
#include <iostream>

/**
 * BossView Constructor
 * Initializes animation variables and sets the initial state tracker to IDLE.
 */
BossView::BossView() : m_animTimer(0), m_currentFrame(0), m_lastState(IDLE) {}

/**
 * Initializes the Boss's graphical assets.
 * Loads the Minotaur sprite sheet and sets up the sprite origin and scale.
 */
void BossView::init() {
    // Attempt to load the sprite sheet from the resources folder
    if(!m_texture.loadFromFile("resources/boss/Minotaur - Sprite Sheet.png")) {
        std::cerr << "Error: Minotaur PNG not found" << std::endl;
    }

    // Set the texture and center the origin for consistent rotation and flipping
    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(48, 48); // Set origin to the center of the 96x96 frame
    m_sprite.setScale(2.5f, 2.5f); // Set the default visual size of the boss
}

/**
 * Updates the Boss's visual state based on the current model data.
 * Handles sprite positioning, horizontal flipping, and animation frame logic.
 * @param dt Delta time since the last frame.
 * @param model Reference to the BossModel containing the current physical state.
 */
void BossView::update(float dt, const BossModel& model) {
    // Retrieve the physical position of the model's hitbox
    sf::Vector2f pos = model.getPosition();

    // VISUAL ADJUSTMENT: Vertical offset (Y-axis).
    // An offset is used to ensure the feet of the sprite align correctly with the ground collision.
    float yOffset = 15.0f;

    // Apply position with the offset to the sprite
    m_sprite.setPosition(pos.x, pos.y + yOffset);

    // SCALE & ORIENTATION: Handle horizontal flipping based on the model's facing direction.
    float s = 2.5f;
    if (model.isFacingRight()) m_sprite.setScale(s, s);
    else m_sprite.setScale(-s, s); // Flip on X-axis

    // STATE CHANGE DETECTION: Reset animation if the Boss switches behavior
    BossState state = model.getState();
    if (state != m_lastState) {
        m_currentFrame = 0;
        m_animTimer = 0;
        m_lastState = state;
    }

    // ANIMATION TIMING: Adjust animation speed depending on the action
    // Attacks play slightly faster (0.08s) than standard movements (0.1s).
    float speed = (state == ATTACKING) ? 0.08f : 0.1f;
    m_animTimer += dt;
    if (m_animTimer >= speed) {
        m_animTimer = 0;
        m_currentFrame++;
    }

    // SPRITE SHEET MAPPING: Define which row to use and how many frames exist per action
    int row = 0, maxFrames = 5;
    switch(state) {
        case IDLE:      row = 0; maxFrames = 5; break;
        case WALKING:   row = 1; maxFrames = 8; break;
        case ATTACKING: row = 3; maxFrames = 9; break;
        case HURT:      row = 6; maxFrames = 3; break;
        case DEAD:      row = 9; maxFrames = 6; break;
    }

    // FRAME CLAMPING: Determine if the animation should loop or freeze on the last frame
    if (state == DEAD || state == ATTACKING) {
        // Linear animations: stay on the final frame once reached
        if(m_currentFrame >= maxFrames) {
            m_currentFrame = maxFrames - 1;
        }
    }
    else {
        // Cyclic animations: Loop back to the first frame for IDLE, WALKING, and HURT
        if(m_currentFrame >= maxFrames) {
            m_currentFrame = 0;
        }
    }

    // Update the texture rectangle to display the correct segment of the sprite sheet
    m_sprite.setTextureRect(sf::IntRect(m_currentFrame * FRAME_SIZE, row * FRAME_SIZE, FRAME_SIZE, FRAME_SIZE));
}

/**
 * Draws the Boss sprite to the provided render window.
 * @param window The SFML RenderWindow target.
 */
void BossView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
