#include "BossView.h"
#include <iostream>

BossView::BossView() : m_animTimer(0), m_currentFrame(0), m_lastState(IDLE) {}

void BossView::init() {
    if(!m_texture.loadFromFile("resources/boss/Minotaur - Sprite Sheet.png")) {
        std::cerr << "Erreur Minotaur PNG" << std::endl;
    }
    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(48, 48);
    m_sprite.setScale(4.0f, 4.0f);
}

void BossView::update(float dt, const BossModel& model) {
    // Récupère la position du modèle (la hitbox physique)
    sf::Vector2f pos = model.getPosition();

    // AJOUT: Décalage visuel vers le bas (Y).
    // Modifie "15.0f" par le nombre de pixels nécessaire pour qu'il touche le sol.
    float yOffset = 15.0f;

    // Applique la position avec le décalage
    m_sprite.setPosition(pos.x, pos.y + yOffset);

    float s = 2.5f;
    if (model.isFacingRight()) m_sprite.setScale(s, s);
    else m_sprite.setScale(-s, s);

    BossState state = model.getState();
    if (state != m_lastState) {
        m_currentFrame = 0;
        m_animTimer = 0;
        m_lastState = state;
    }

    float speed = (state == ATTACKING) ? 0.08f : 0.1f;
    m_animTimer += dt;
    if (m_animTimer >= speed) {
        m_animTimer = 0;
        m_currentFrame++;
    }

    int row = 0, maxFrames = 5;
    switch(state) {
        case IDLE: row=0; maxFrames=5; break;
        case WALKING: row=1; maxFrames=8; break;
        case ATTACKING: row=3; maxFrames=9; break;
        case HURT: row=6; maxFrames=3; break;
        case DEAD: row=9; maxFrames=6; break;
    }

    if (state == DEAD || state == ATTACKING) {
        if(m_currentFrame >= maxFrames) {
            m_currentFrame = maxFrames - 1; // On reste sur la dernière image
        }
    }
    else {
        // Pour IDLE, WALKING, HURT : On boucle
        if(m_currentFrame >= maxFrames) {
            m_currentFrame = 0; // On recommence au début
        }
    }

    m_sprite.setTextureRect(sf::IntRect(m_currentFrame * FRAME_SIZE, row * FRAME_SIZE, FRAME_SIZE, FRAME_SIZE));
}

void BossView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
