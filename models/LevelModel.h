#ifndef LEVEL_MODEL_H
#define LEVEL_MODEL_H

#include <vector>
#include <string>
#include <SFML/Graphics/Rect.hpp>

class LevelModel {
public:
    LevelModel();
    void loadLevel(); // Charge la map ASCII

    // Renvoie la liste des murs autour d'une position (pour collision optimisée)
    std::vector<sf::FloatRect> getNearbyWalls(float x, float y) const;

    // Données brutes pour la Vue
    const std::vector<std::string>& getMapData() const;
    float getTileSize() const;

private:
    std::vector<std::string> m_mapData;
    float m_tileSize;
    float m_scale;
};
#endif
