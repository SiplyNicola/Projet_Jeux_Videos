#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>

// 1. L'ENUM D'IDENTITÉ (INTROSPECTION)
enum class EntityType {
    PLAYER,
    SNAKE,
    SPIDER,
    BOSS
};

class Character : public Entity {
public:

    Character(int hp, float speed, int attackDamage, EntityType type);

    // 3. DESTRUCTEUR VIRTUEL (Obligatoire pour Coplien/Polymorphisme)
    virtual ~Character() = default;

    virtual void takeDamage(int amount);
    virtual sf::FloatRect getHitbox() const = 0;

    int getHP() const { return m_hp; }
    void setHP(int hp);
    bool isDead() const { return m_hp <= 0; }
    int getAttackDamage() const { return m_attackDamage; }

    // 4. MÉTHODE D'INTROSPECTION
    EntityType getType() const { return m_type; }

protected:
    int m_hp;
    float m_speed;
    int m_attackDamage;

    // Variable pour stocker qui je suis
    EntityType m_type;
};

#endif
