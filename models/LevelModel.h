#ifndef LEVEL_MODEL_H
#define LEVEL_MODEL_H

#include <vector>
#include <string>
#include <SFML/Graphics/Rect.hpp>

class LevelModel {
public:
    LevelModel();
    // Ajout du paramètre levelId (1 par défaut)
    void loadLevel(int levelId); // Charge la map ASCII
    int getLevelId() const { return m_levelId; }

    // Renvoie la liste des murs autour d'une position (pour collision optimisée)
    std::vector<sf::FloatRect> getNearbyWalls(float x, float y) const;

    // Données brutes pour la Vue
    const std::vector<std::string>& getMapData() const;
    float getTileSize() const;
    float getScale() const;

private:
    std::vector<std::string> m_mapData;
    float m_tileSize;
    float m_scale;
    int m_levelId;
};
#endif
