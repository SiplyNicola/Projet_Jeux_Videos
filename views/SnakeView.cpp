#include "SnakeView.h"
#include <iostream>

/**
 * SnakeView Constructor
 * Initializes animation timers and sets the starting frame index.
 */
SnakeView::SnakeView() : m_animTimer(0), m_currentFrame(0) {}

/**
 * Static Texture Definition
 * Shared across all instances of SnakeView to optimize memory usage by only loading
 * the sprite sheet once into GPU memory.
 */
sf::Texture SnakeView::m_texture;

/**
 * Initializes the snake's graphical assets.
 * Implements a check to ensure the texture is only loaded from disk once.
 */
void SnakeView::init() {
    // MEMORY OPTIMIZATION: Only load the texture if it hasn't been initialized yet.
    // This prevents redundant disk reads if multiple snakes are created.
    if (m_texture.getSize().x == 0) {
        if (!m_texture.loadFromFile("resources/animals/spritesheet.png")) {
             // Fallback path in case the standard resources folder is misplaced.
             if (!m_texture.loadFromFile("spritesheet.png")) {
                 std::cerr << "ERROR: Snake texture not found!" << std::endl;
             }
        }
    }

    // Link the sprite to the static shared texture.
    m_sprite.setTexture(m_texture);

    // Set the origin to the bottom-center of the frame (pivot point).
    m_sprite.setOrigin(FRAME_SIZE / 2.0f, (float)FRAME_SIZE);

    // Default visual scale.
    m_sprite.setScale(2.0f, 2.0f);
}

/**
 * Updates the visual representation of the snake based on the current model data.
 * Handles sprite positioning, horizontal mirroring, and animation cycling.
 * @param dt Delta time since the last frame.
 * @param model Reference to the SnakeModel containing logic and orientation.
 */
void SnakeView::update(float dt, const SnakeModel& model) {
    // Synchronize sprite position with the physical model's world coordinates.
    m_sprite.setPosition(model.getPosition());

    // 1. ORIENTATION MANAGEMENT (MIRRORING)
    // If facing right: Use positive scale.
    // If facing left: Use negative X scale to create a mirror effect.
    if (model.isFacingRight()) {
        m_sprite.setScale(2.0f, 2.0f);
    } else {
        m_sprite.setScale(-2.0f, 2.0f);
    }

    // 2. ANIMATION LOGIC
    // Animation continues as long as the snake is not in the DEATH state.
    if (model.getState() != SnakeState::DEATH) {
        m_animTimer += dt;

        // Frame duration is set to 0.2 seconds.
        if (m_animTimer >= 0.2f) {
            m_animTimer = 0;
            m_currentFrame++;

            // Loop back to the first frame after frame index 3 (4 frames total).
            if (m_currentFrame >= 4) m_currentFrame = 0;
        }
    }

    // 3. TEXTURE RECTANGLE CLIPPING
    // The green snake is always located on row 0 of the sprite sheet.
    int row = 0;

    // Slices the correct frame from the sprite sheet.
    // Horizontal flipping is handled by setScale(-X, Y), so coordinates remain standard.
    m_sprite.setTextureRect(sf::IntRect(
        m_currentFrame * FRAME_SIZE,
        row * FRAME_SIZE,
        FRAME_SIZE,
        FRAME_SIZE
    ));
}

/**
 * Renders the snake sprite to the active window.
 * @param window The SFML RenderWindow target.
 */
void SnakeView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
