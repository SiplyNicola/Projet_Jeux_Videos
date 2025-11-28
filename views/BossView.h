#ifndef BOSS_VIEW_H
#define BOSS_VIEW_H

#include <SFML/Graphics.hpp>
#include "../models/BossModel.h"

class BossView {
public:
    BossView();
    void init();
    void update(float dt, const BossModel& model);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    float m_animTimer;
    int m_currentFrame;
    BossState m_lastState;
    const int FRAME_SIZE = 96;
};
#endif
