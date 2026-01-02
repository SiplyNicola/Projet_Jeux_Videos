#include "Entity.h"

/**
 * Entity Constructor
 * Serves as the base class for all physical objects in the game world,
 * including players, enemies, and bosses.
 */
Entity::Entity()
    : m_position(0.0f, 0.0f),  // Initialize world coordinates to the origin (0,0)
      m_velocity(0.0f, 0.0f),  // Ensure the entity starts with no movement
      m_facingRight(true)      // Set default orientation to look toward the right
{}
