#include "Loot.h"
#include <cmath>

Loot::Loot(std::string type, sf::Vector2f pos)
    : m_type(type), m_collected(false), m_floatingTimer(0.0f)
{
    m_position = pos;
    m_startY = pos.y;
    m_velocity = {0.f, 0.f};
    m_facingRight = true;
}

void Loot::update(float dt) {
    if (m_collected) return;

    // --- ANIMATION DE FLOTTEMENT ---
    // On utilise un sinus pour faire monter et descendre l'objet doucement
    m_floatingTimer += dt * 3.0f; // Vitesse de l'oscillation
    float offset = std::sin(m_floatingTimer) * 10.0f; // Amplitude de 10 pixels

    m_position.y = m_startY + offset;
}

sf::FloatRect Loot::getHitbox() const {
    // Une hitbox de 30x30 pixels centrée sur l'objet
    return sf::FloatRect(m_position.x - 15.f, m_position.y - 15.f, 30.f, 30.f);
}
