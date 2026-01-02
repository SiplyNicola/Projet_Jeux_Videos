#include "Character.h"

/**
 * Character Constructor
 * Initializes base statistics and identity for all living entities (Snakes, Spiders, etc.).
 * * @param hp The initial health points of the character.
 * @param speed The movement speed used by the character's AI or controller.
 * @param attackDamage The amount of damage this character deals to the player.
 * @param type The EntityType identifier used for runtime type checking (Introspection).
 */
Character::Character(int hp, float speed, int attackDamage, EntityType type)
    : Entity(),
      m_hp(hp),
      m_speed(speed),
      m_attackDamage(attackDamage),
      m_type(type) // Identity is stored here to allow the Game class to apply specific logic per type.
{}


void Character::takeDamage(int amount) {
    m_hp -= amount;

    // Safety check: Ensure health points do not drop below zero.
    if (m_hp < 0) m_hp = 0;
}
