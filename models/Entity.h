#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class Entity
{
    public:
        Entity(int hp, float speed, int attackDamage);
        virtual ~Entity() = default;

        virtual void update(float dt) = 0;
        virtual sf::FloatRect getHitbox() const = 0;
        virtual void takeDamage(int amount) = 0;

        sf::Vector2f getPosition() const { return m_position; }
        int getHP() const { return m_hp; }
        bool isFacingRight() const { return m_facingRight; }
        bool isDead() const { return m_hp <= 0; }
        int getAttackDamage() const { return m_attackDamage; }


        void setPosition(float x, float y) { m_position = {x, y}; }
        void setVelocity(sf::Vector2f v) { m_velocity = v; }
        sf::Vector2f getVelocity() const { return m_velocity; }

   protected:
    int m_hp; // Mettre en premier
    float m_speed;
    int m_attackDamage;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    bool m_facingRight;

};

#endif // ENTITY_H
