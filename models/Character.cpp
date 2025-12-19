#include "Character.h"

Character::Character(int hp, float speed, int attackDamage)
    : Entity(), m_hp(hp), m_speed(speed), m_attackDamage(attackDamage)
{}

void Character::takeDamage(int amount) {
    m_hp -= amount;
    if (m_hp < 0) m_hp = 0;
}
