#include "SnakeView.h"
#include <iostream>

SnakeView::SnakeView() : m_animTimer(0), m_currentFrame(0) {}

void SnakeView::init() {
    if (!m_texture.loadFromFile("resources/animals/spritesheet.png")) {
        std::cerr << "ERREUR : Image introuvable !" << std::endl;
    }
    m_sprite.setTexture(m_texture);

    // Origine au centre bas (pour que le miroir fonctionne bien)
    m_sprite.setOrigin(FRAME_SIZE / 2.0f, (float)FRAME_SIZE);

    // On agrandit x4 car l'image est toute petite (16px)
    m_sprite.setScale(4.0f, 4.0f);
}

void SnakeView::update(float dt, const SnakeModel& model) {
    m_sprite.setPosition(model.getPosition());

    // 1. GESTION DU REGARD (MIROIR)
    // Si regarde à droite : Echelle positive
    // Si regarde à gauche : Echelle négative en X (Miroir)
    if (model.isFacingRight()) {
        m_sprite.setScale(4.0f, 4.0f);
    } else {
        m_sprite.setScale(-4.0f, 4.0f);
    }

    // 2. Animation
    if (model.getState() != SnakeState::DEATH) {
        m_animTimer += dt;
        if (m_animTimer >= 0.2f) {
            m_animTimer = 0;
            m_currentFrame++;
            if (m_currentFrame >= 4) m_currentFrame = 0;
        }
    }

    // 3. Découpe de l'image (Toujours ligne 0 pour le serpent vert)
    int row = 0;

    // On prend toujours les colonnes de base, le setScale(-4, 4) fera l'inversion visuelle
    m_sprite.setTextureRect(sf::IntRect(
        m_currentFrame * FRAME_SIZE,
        row * FRAME_SIZE,
        FRAME_SIZE,
        FRAME_SIZE
    ));
}

void SnakeView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
