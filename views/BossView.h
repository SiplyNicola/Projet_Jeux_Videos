#pragma once
#include <SFML/Graphics.hpp>
#include "BossModel.h"

class BossView {
public:
    BossView();

    // Charge la texture du boss
    void init();

    // Met à jour l'animation en fonction de l'état du modèle
    void update(float dt, const BossModel& model);

    // Dessine le boss à l'écran
    void draw(sf::RenderWindow& window);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    // Variables pour gérer l'animation
    float m_animTimer;
    int m_currentFrame;
    BossState m_lastState; // Pour détecter quand l'état change et reset l'anim

    // Taille d'une case dans le sprite sheet
    // D'après ton .cpp (setOrigin 48,48), la taille est de 96x96
    const int FRAME_SIZE = 96;
};
