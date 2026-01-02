#ifndef ENEMYMODEL_H
#define ENEMYMODEL_H

#include "Character.h"

/**
 * @class EnemyModel
 * @brief Base class for all standard ground enemies (Rats, Snakes, etc.).
 * * INTERESTING ASPECT: CODE REUSE / FACTORIZATION
 * This class serves as a "toolkit" to avoid code duplication.
 * Instead of rewriting physics (gravity) and AI (patrol) in every monster,
 * EnemyModel provides these ready-to-use methods.
 */
class EnemyModel : public Character {
public:
    /**
     * @brief EnemyModel Constructor.
     * @param hp Enemy Health Points.
     * @param speed Movement speed (pixels/sec).
     * @param damage Damage inflicted on the player.
     * @param type Unique identifier (SNAKE, RAT, SPIDER...) for introspection.
     */
    EnemyModel(int hp, float speed, int damage, EntityType type);

    // --- UTILITY METHODS (Available for child classes) ---

    /**
     * @brief Applies standard gravity to the entity.
     * @param dt Elapsed time (Delta Time) for framerate independence.
     * * This method centralizes the falling formula.
     * Child classes (e.g., SpiderModel) can choose to call it or not.
     */
    void applyGravity(float dt);

    /**
     * @brief Handles automatic patrol (Back and Forth).
     * @param dt Elapsed time.
     * * This method manages the timer, direction change, and X velocity.
     * It greatly simplifies the code of child classes.
     */
    void updatePatrolMovement(float dt);

public:
    // These variables are 'protected' to be accessible by SnakeModel, RatModel, etc.
    float m_patrolTimer;      // Internal timer for patrol
    float m_patrolDuration;   // Time before changing direction
    bool m_movingRight;       // Current direction
};

#endif
