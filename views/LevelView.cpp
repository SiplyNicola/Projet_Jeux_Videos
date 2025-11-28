#include "LevelView.h"
#include <iostream>

void LevelView::init() {
    if(!m_tileset.loadFromFile("resources/oak_woods_v1.0/oak_woods_tileset.png")) {
        std::cerr << "Erreur Tileset" << std::endl;
    }
}

void LevelView::build(const LevelModel& model) {
    m_sprites.clear();
    const auto& map = model.getMapData();
    float size = model.getTileSize();
    float scale = size / 24.0f; // Ratio vs 24px original

    for(int y=0; y<map.size(); y++) {
        for(int x=0; x<map[y].size(); x++) {
            char cell = map[y][x];
            if (cell == ' ') continue;

            sf::Sprite s;
            s.setTexture(m_tileset);
            s.setScale(scale, scale);
            s.setPosition(x * size, y * size);

            if (cell == '1') s.setTextureRect(sf::IntRect(24, 0, 24, 24)); // Herbe
            else if (cell == '2') s.setTextureRect(sf::IntRect(24, 24, 24, 24)); // Terre

            m_sprites.push_back(s);
        }
    }
}

void LevelView::draw(sf::RenderWindow& window) {
    for(const auto& s : m_sprites) window.draw(s);
}
