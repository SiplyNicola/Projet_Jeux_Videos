#ifndef HEALTHPOTIONVIEW_H
#define HEALTHPOTIONVIEW_H

#include <SFML/Graphics.hpp>
#include <iostream>

class HealthPotionView {
public:
    HealthPotionView() = default;

    void init();

    void draw(sf::RenderWindow& window, sf::Vector2f position);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};

#endif
