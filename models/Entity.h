#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Vector2.hpp>

class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;

    sf::Vector2f getPosition() const { return m_position; }
    void setPosition(float x, float y) { m_position = {x, y}; }

    sf::Vector2f getVelocity() const { return m_velocity; }
    void setVelocity(sf::Vector2f v) { m_velocity = v; }

    bool isFacingRight() const { return m_facingRight; }

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    bool m_facingRight;
};

#endif
