#ifndef WORLDTRANSITIONMODEL_H
#define WORLDTRANSITIONMODEL_H

#include <string>

/**
 * WorldTransitionModel
 * Handles the logical state and timing of the transition screen.
 */
class WorldTransitionModel {
public:
    WorldTransitionModel();

    /**
     * Prepares the model with new level data and life count.
     */
    void reset(std::string levelName, int lives);

    /**
     * Updates the internal timer to track screen duration.
     */
    void update(float dt);

    // Accessors
    bool isFinished() const;
    std::string getLevelName() const;
    int getLives() const;

private:
    std::string m_levelName;       // Text string for the current world
    int         m_livesRemaining;  // Remaining life count to show on screen
    float       m_displayTimer;    // Accumulated time since the screen appeared
    float       m_displayDuration; // Total time the screen should remain visible
    bool        m_isFinished;      // Boolean flag used by Game.cpp to trigger respawn
};

#endif
