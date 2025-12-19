#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>

class Character : public Entity {
public:
    Character(int hp, float speed, int attackDamage);

    virtual void takeDamage(int amount);
    virtual sf::FloatRect getHitbox() const = 0;

    int getHP() const { return m_hp; }
    bool isDead() const { return m_hp <= 0; }
    int getAttackDamage() const { return m_attackDamage; }

protected:
    int m_hp;
    float m_speed;
    int m_attackDamage;
};

#endif
