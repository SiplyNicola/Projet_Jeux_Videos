#include "PlantModel.h"

/**
 * PlantModel Constructor
 * Initializes the carnivorous plant at a specific world position.
 * @param pos The initial X and Y coordinates for the plant.
 */
PlantModel::PlantModel(sf::Vector2f pos) {
    m_position = pos;    // Set the world position
    m_state = P_IDLE;    // Start in the waiting state
    m_timer = 0;         // Reset the state transition timer
}

/**
 * Updates the plant's internal state machine based on the elapsed time.
 * Cycles between waiting (IDLE) and lunging (ATTACKING).
 * @param dt Delta time since the last frame.
 */
void PlantModel::update(float dt) {
    m_timer += dt; // Increment the timer by the frame delta

    if (m_state == P_IDLE) {
        // Wait time: The plant remains passive for 3 seconds before attacking
        if (m_timer >= 3.0f) {
            m_state = P_ATTACKING;
            m_timer = 0.0f; // Reset timer for the attack duration
        }
    }
    else if (m_state == P_ATTACKING) {
        // Attack duration: The attack lasts for 1.0s.
        // This duration is slightly extended to ensure the graphical view can
        // display the final frame (frame 7) without being cut off prematurely.
        if (m_timer >= 1.0f) {
            m_state = P_IDLE;
            m_timer = 0.0f; // Reset timer to return to the waiting cycle
        }
    }
}

/**
 * Calculates and returns the attack collision zone (bite zone).
 * This area is checked in the Game class to detect player hits.
 * @return A square FloatRect (80x80) centered around the plant's position.
 */
sf::FloatRect PlantModel::getBiteZone() const {
    // Defines the bite area centered on the plant's position
    return sf::FloatRect(m_position.x - 40, m_position.y - 40, 80, 80);
}
