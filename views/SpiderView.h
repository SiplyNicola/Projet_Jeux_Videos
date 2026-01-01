#ifndef SPIDERVIEW_H
#define SPIDERVIEW_H

#include <SFML/Graphics.hpp>
#include "../models/SpiderModel.h"

class SpiderView {
public:
    SpiderView();

    void init();

    // Met à jour l'animation et la position du sprite
    void update(float dt, const SpiderModel& model);

    // Dessine le sprite ET le fil (c'est pourquoi on a besoin du "model" ici)
    void draw(sf::RenderWindow& window, const SpiderModel& model);

private:
    sf::Sprite m_sprite;
    static sf::Texture m_texture; // "static" pour ne charger l'image qu'une seule fois en mémoire

    float m_animTimer;
    int m_currentFrame;

    // Taille d'une case de l'araignée (32x32 pixels)
    const int FRAME_SIZE = 32;
};

#endif
