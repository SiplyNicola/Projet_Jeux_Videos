#include "SpiderView.h"
#include <iostream>

/**
 * Static Texture Definition
 * Shared across all instances of SpiderView to optimize GPU memory by loading
 * the spider sprite sheet only once.
 */
sf::Texture SpiderView::m_texture;

/**
 * SpiderView Constructor
 * Initializes animation timing and the starting frame index.
 */
SpiderView::SpiderView() : m_animTimer(0), m_currentFrame(0) {}

/**
 * Initializes the spider's graphical assets.
 * Loads the texture from disk if it hasn't been initialized yet.
 */
void SpiderView::init() {
    // MEMORY OPTIMIZATION: Only perform disk I/O if the static texture is empty.
    if (m_texture.getSize().x == 0) {
        if (!m_texture.loadFromFile("resources/animals/spiderspritesheet.png")) {
             std::cerr << "ERROR: Spider texture not found!" << std::endl;
        }
    }

    // Bind the sprite to the shared static texture.
    m_sprite.setTexture(m_texture);

    // Set the pivot point (origin) to the bottom-center of the frame (at the feet).
    m_sprite.setOrigin(FRAME_SIZE / 2.0f, (float)FRAME_SIZE);

    // Set the initial visual scale.
    m_sprite.setScale(2.0f, 2.0f);
}

/**
 * Updates the visual state of the spider based on model data.
 * Handles position synchronization, horizontal/vertical mirroring, and animation frame logic.
 * @param dt Delta time since the last frame.
 * @param model Reference to the SpiderModel containing logic and state.
 */
void SpiderView::update(float dt, const SpiderModel& model) {
    // Synchronize sprite position with the physical model.
    m_sprite.setPosition(model.getPosition());

    // 1. ORIENTATION MANAGEMENT (MIRRORING)
    // scaleX handles horizontal mirroring based on facing direction.
    float scaleX = model.isFacingRight() ? -2.0f : 2.0f;
    float scaleY = 2.0f;

    // VERTICAL ORIENTATION:
    // If the spider is hanging from the ceiling or dropping, we invert it vertically.
    // This allows us to use the same walking frames upside down.
    if (model.getState() == SpiderState::HANGING || model.getState() == SpiderState::DROPPING) {
        scaleY = -2.0f; // Inverts the sprite on the Y-axis.
    }

    m_sprite.setScale(scaleX, scaleY);

    // 2. ANIMATION TIMING
    // Progress frames as long as the spider is not in the DEATH state.
    if (model.getState() != SpiderState::DEATH) {
        m_animTimer += dt;
        if (m_animTimer >= 0.15f) { // Animation speed set to 0.15s per frame.
            m_animTimer = 0;
            m_currentFrame++;
            // Cycle back to the first frame after frame 4 (5 frames total).
            if (m_currentFrame >= 5) m_currentFrame = 0;
        }
    }

   // 3. SPRITE SHEET ROW SELECTION
   // Determines which row to sample from the sheet based on the current AI state.
    int row = 0;
    switch (model.getState()) {
        case SpiderState::HANGING:
            row = 0; // Use normal walk frames, inverted by the scaleY logic above.
            break;

        case SpiderState::DROPPING:
            row = 0; // Maintain walking look while falling.
            break;

        case SpiderState::WALK:
            row = 0;
            break;

        case SpiderState::ATTACK:
            row = 2; // Jump to the attack animation row.
            break;

        case SpiderState::DEATH:
            row = 4; // Use the death animation row.
            // Clamping: stay on the final death frame.
            if (m_currentFrame >= 4) m_currentFrame = 4;
            break;

        default:
            row = 0;
            break;
    }

    // Update the texture rectangle to display the correct frame from the spritesheet.
    m_sprite.setTextureRect(sf::IntRect(
        m_currentFrame * FRAME_SIZE,
        row * FRAME_SIZE,
        FRAME_SIZE,
        FRAME_SIZE
    ));
}

/**
 * Custom draw function for the Spider.
 * Renders the silk thread (line) and then the spider sprite.
 * @param window The SFML RenderWindow target.
 * @param model Reference to the model to access anchor points for the silk thread.
 */
void SpiderView::draw(sf::RenderWindow& window, const SpiderModel& model) {
    // 1. SILK THREAD RENDERING
    // Only draw the silk thread if the spider is attached to the ceiling or dropping.
    if (model.getState() == SpiderState::HANGING || model.getState() == SpiderState::DROPPING) {
        // Draw a simple line from the ceiling anchor to the current spider position.
        sf::Vertex line[] = {
            // Anchor point (Ceiling)
            sf::Vertex(sf::Vector2f(model.getPosition().x, model.getAnchorY()), sf::Color(200, 200, 200)),
            // Current position (Spider)
            sf::Vertex(sf::Vector2f(model.getPosition().x, model.getPosition().y), sf::Color(255, 255, 255))
        };
        window.draw(line, 2, sf::Lines);
    }

    // 2. RENDER THE SPIDER SPRITE
    window.draw(m_sprite);
}
