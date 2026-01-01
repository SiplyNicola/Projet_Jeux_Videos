#include "SpiderView.h"
#include <iostream>

sf::Texture SpiderView::m_texture;

SpiderView::SpiderView() : m_animTimer(0), m_currentFrame(0) {}

void SpiderView::init() {
    if (m_texture.getSize().x == 0) {
        if (!m_texture.loadFromFile("resources/animals/spiderspritesheet.png")) {
             std::cerr << "ERREUR TEXTURE SPIDER" << std::endl;
        }
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(FRAME_SIZE / 2.0f, (float)FRAME_SIZE); // Origine aux pieds
    m_sprite.setScale(2.0f, 2.0f);
}

void SpiderView::update(float dt, const SpiderModel& model) {
    m_sprite.setPosition(model.getPosition());

    // 1. GESTION DE L'ORIENTATION (VERS LE HAUT OU LE BAS)
    float scaleX = model.isFacingRight() ? -2.0f : 2.0f; // Miroir gauche/droite
    float scaleY = 2.0f;

    // SI ELLE EST AU PLAFOND OU TOMBE : TÊTE EN BAS
    if (model.getState() == SpiderState::HANGING || model.getState() == SpiderState::DROPPING) {
        scaleY = -2.0f; // <--- C'est ça qui la met à l'envers !
    }

    m_sprite.setScale(scaleX, scaleY);


    // 2. ANIMATION
    if (model.getState() != SpiderState::DEATH) {
        m_animTimer += dt;
        if (m_animTimer >= 0.15f) {
            m_animTimer = 0;
            m_currentFrame++;
            if (m_currentFrame >= 5) m_currentFrame = 0;
        }
    }


   // 3. CHOIX LIGNE SPRITESHEET
    int row = 0;
    switch (model.getState()) {
        case SpiderState::HANGING:
            row = 0; // <--- CHANGE ICI (Mets 0 au lieu de 5)
            // On utilise l'image normale, elle sera juste inversée par le code scaleY
            break;

        case SpiderState::DROPPING:
            row = 0;
            break;

        case SpiderState::WALK:
            row = 0; // Ou 1 si tu veux varier la marche
            break;

        case SpiderState::ATTACK:
            row = 2;
            break;

        case SpiderState::DEATH:
            row = 4;
            if (m_currentFrame >= 4) m_currentFrame = 4;
            break;

        default:
            row = 0;
            break;
    }

    m_sprite.setTextureRect(sf::IntRect(
        m_currentFrame * FRAME_SIZE,
        row * FRAME_SIZE,
        FRAME_SIZE,
        FRAME_SIZE
    ));
}

void SpiderView::draw(sf::RenderWindow& window, const SpiderModel& model) {
    // 1. DESSIN DU FIL (Seulement si accrochée ou en train de tomber)
    if (model.getState() == SpiderState::HANGING || model.getState() == SpiderState::DROPPING) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(model.getPosition().x, model.getAnchorY()), sf::Color(200, 200, 200)),
            sf::Vertex(sf::Vector2f(model.getPosition().x, model.getPosition().y), sf::Color(255, 255, 255))
        };
        window.draw(line, 2, sf::Lines);
    }

    // 2. DESSIN SPRITE
    window.draw(m_sprite);
}
