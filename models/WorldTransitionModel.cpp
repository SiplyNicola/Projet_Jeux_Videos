#include "WorldTransitionModel.h"

/**
 * WorldTransitionModel Constructor
 * Sets default values for the timer and display duration.
 */
WorldTransitionModel::WorldTransitionModel()
    : m_displayTimer(0.0f),
      m_displayDuration(2.5f),
      m_isFinished(false),
      m_livesRemaining(1)
{
}

/**
 * Resets the transition data.
 * @param levelName The text identifying the level (e.g., "WORLD 1-1").
 * @param lives The number of lives to display.
 */
void WorldTransitionModel::reset(std::string levelName, int lives) {
    m_levelName = levelName;
    m_livesRemaining = lives;
    m_displayTimer = 0.0f;
    m_isFinished = false;
}

/**
 * Progresses the display timer.
 * Changes the m_isFinished flag once the duration is reached.
 * @param dt Delta time from the game loop.
 */
void WorldTransitionModel::update(float dt) {
    // If the transition is already done, do nothing
    if (m_isFinished) return;

    m_displayTimer += dt;

    // Check if the timer has exceeded the 2.5s duration
    if (m_displayTimer >= m_displayDuration) {
        m_isFinished = true;
    }
}

/**
 * Returns true if the transition screen is done displaying.
 */
bool WorldTransitionModel::isFinished() const {
    return m_isFinished;
}

/**
 * Returns the stored level name.
 */
std::string WorldTransitionModel::getLevelName() const {
    return m_levelName;
}

/**
 * Returns the stored life count.
 */
int WorldTransitionModel::getLives() const {
    return m_livesRemaining;
}
