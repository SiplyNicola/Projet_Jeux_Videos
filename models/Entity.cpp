#include "Entity.h"

Entity::Entity(int hp, float speed, int attackDamage)
    : m_hp(hp),
      m_speed(speed),
      m_attackDamage(attackDamage), // <--- INITIALIS�
      m_position(0.0f, 0.0f),
      m_velocity(0.0f, 0.0f),
      m_facingRight(true)
{
}
