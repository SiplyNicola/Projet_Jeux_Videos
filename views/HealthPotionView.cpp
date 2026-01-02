#include "HealthPotionView.h"

void HealthPotionView::init() {
    // Chargement de l'image (nom du fichier mis à jour)
    if (!m_texture.loadFromFile("resources/healPotion/healPotion.png")) {
        std::cerr << "ERROR: Impossible de charger resources/healPotion.png" << std::endl;
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    m_sprite.setOrigin(16.0f, 16.0f);
    m_sprite.setScale(1.5f, 1.5f);
}

void HealthPotionView::draw(sf::RenderWindow& window, sf::Vector2f position) {
    m_sprite.setPosition(position);
    window.draw(m_sprite);
}
