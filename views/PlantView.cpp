#include "PlantView.h"
#include <algorithm> // For std::min

/**
 * PlantView Constructor
 * Initializes animation properties, setting the total frame count to 8
 * and the default animation cycle duration to 1 second.
 */
PlantView::PlantView() : m_frameCount(8), m_animDuration(1.0f) {}

/**
 * Initializes the graphical components of the plant.
 * Loads the sprite sheet, sets the origin for correct positioning, and applies scaling.
 */
void PlantView::init() {
    // Load the full attack animation sprite sheet from the resources folder
    m_texture.loadFromFile("resources/Plant3/Attack/Plant3_Attack_full.png");
    m_sprite.setTexture(m_texture);

    // Square frame dimensions (64x64 pixels)
    int frameW = 64;

    // Set the origin to the bottom-center of the frame (x=32, y=64)
    // This ensures the plant is anchored to the ground at its base.
    m_sprite.setOrigin(frameW / 2.0f, (float)frameW);

    // Scale the plant sprite up by 3x
    m_sprite.setScale(3.0f, 3.0f);
}

/**
 * Updates the plant's visual animation based on the current model state and timer.
 *
 * @param dt Delta time since the last frame.
 * @param model Reference to the PlantModel containing state and timing logic.
 */
void PlantView::update(float dt, const PlantModel& model) {
    // Update the sprite position to match the physical model's world position
    m_sprite.setPosition(model.getPosition());

    const int frameW = 64; // Width of a single animation frame
    int currentFrame = 0;

    // ANIMATION LOGIC: Only animate if the plant is in the ATTACKING state
    if (model.getState() == P_ATTACKING) {
        // Calculate animation progress as a ratio (0.0 to 1.0)
        float progress = model.getTimer() / m_animDuration;

        // Map progress to the frame index (0 to 7)
        currentFrame = static_cast<int>(progress * m_frameCount);

        // --- BOUNDARY PROTECTION ---
        // If the calculation reaches the end of the duration (ratio = 1.0),
        // we lock the animation to the final frame (index 7).
        if (currentFrame >= m_frameCount) {
            currentFrame = m_frameCount - 1;
        }
    } else {
        // In IDLE state, the plant displays frame 0 (mouth closed)
        currentFrame = 0;
    }

    // Safety: Ensure the frame index never drops below zero
    if (currentFrame < 0) currentFrame = 0;

    // TEXTURE RECT UPDATE
    // Calculate the horizontal pixel offset for the current frame in the sprite sheet
    int frameX = currentFrame * frameW;

    // Apply the clipping rectangle to the sprite
    m_sprite.setTextureRect(sf::IntRect(frameX, 0, frameW, 64));
}

/**
 * Draws the plant sprite to the active render window.
 * @param window The SFML RenderWindow target.
 */
void PlantView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
