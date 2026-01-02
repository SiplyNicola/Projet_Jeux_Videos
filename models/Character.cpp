#include "Character.h"

// Mise à jour du constructeur pour accepter et stocker 'type'
Character::Character(int hp, float speed, int attackDamage, EntityType type)
    : Entity(),
      m_hp(hp),
      m_speed(speed),
      m_attackDamage(attackDamage),
      m_type(type) // <--- On enregistre l'identité ici
{}

void Character::takeDamage(int amount) {
    m_hp -= amount;
    if (m_hp < 0) m_hp = 0;
}
