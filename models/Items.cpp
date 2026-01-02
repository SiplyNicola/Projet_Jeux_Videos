#include "Items.h"

Items::Items(sf::Vector2f position, std::string name)
    : m_position(position), m_name(name) {}

sf::FloatRect Items::getHitbox() const {
    return sf::FloatRect(m_position.x - 16.0f, m_position.y - 16.0f, 32.0f, 32.0f);
}
