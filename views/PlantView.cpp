#include "PlantView.h"
#include <algorithm> // Pour std::min

PlantView::PlantView() : m_frameCount(8), m_animDuration(1.0f) {}

void PlantView::init() {
    m_texture.loadFromFile("resources/Plant3/Attack/Plant3_Attack_full.png");
    m_sprite.setTexture(m_texture);

    // Taille d'une frame carrée (64x64)
    int frameW = 64;
    m_sprite.setOrigin(frameW / 2.0f, (float)frameW);
    m_sprite.setScale(3.0f, 3.0f); // Taille de la plante
}

void PlantView::update(float dt, const PlantModel& model) {
    m_sprite.setPosition(model.getPosition());

    const int frameW = 64;
    int currentFrame = 0;

    if (model.getState() == P_ATTACKING) {
        // On calcule la progression
        float progress = model.getTimer() / m_animDuration;

        // On multiplie par 8 (frameCount)
        currentFrame = static_cast<int>(progress * m_frameCount);

        // --- PROTECTION ULTIME ---
        // Si le calcul arrive à 8 (fin pile de l'anim), on reste bloqué sur la frame 7
        if (currentFrame >= m_frameCount) {
            currentFrame = m_frameCount - 1;
        }
    } else {
        // En IDLE, on affiche l'image 0 (bouche fermée)
        currentFrame = 0;
    }

    // Sécurité : currentFrame ne doit jamais être négatif
    if (currentFrame < 0) currentFrame = 0;

    // Mise à jour du rectangle de découpe
    int frameX = currentFrame * frameW;
    m_sprite.setTextureRect(sf::IntRect(frameX, 0, frameW, 64));
}
void PlantView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
