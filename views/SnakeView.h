#ifndef SNAKEVIEW_H
#define SNAKEVIEW_H

#include <SFML/Graphics.hpp>
#include "../models/SnakeModel.h"

class SnakeView {
public:
    SnakeView();

    void init();
    void update(float dt, const SnakeModel& model);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite m_sprite;
    static sf::Texture m_texture;

    float m_animTimer;
    int m_currentFrame;

    // IMPORTANT : 16 pixels car tes sprites sont petits
    const int FRAME_SIZE = 16;
};

#endif
