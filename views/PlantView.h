#ifndef PLANT_VIEW_H
#define PLANT_VIEW_H

#include <SFML/Graphics.hpp>
#include "../models/PlantModel.h"

class PlantView {
public:
    PlantView();

    // Charge la texture de la plante
    void init();

    // Met à jour l'image affichée (animation)
    void update(float dt, const PlantModel& model);

    // Dessine la plante à l'écran
    void draw(sf::RenderWindow& window);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    // Paramètres d'animation
    int m_frameCount;
    float m_animDuration;
};

#endif
