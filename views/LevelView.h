#ifndef LEVEL_VIEW_H
#define LEVEL_VIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../models/LevelModel.h"

class LevelView {
public:
    void init();
    void build(const LevelModel& model); // Construit les sprites
    void draw(sf::RenderWindow& window);

private:
    sf::Texture m_tileset;
    std::vector<sf::Sprite> m_sprites;
};
#endif
