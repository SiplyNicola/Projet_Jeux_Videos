#include "LevelModel.h"

LevelModel::LevelModel() {
    m_tileSize = 24.0f; // Taille de base du tileset
    m_scale = 3.0f;     // Zoom global
}

void LevelModel::loadLevel() {
    // Ton niveau en ASCII
    m_mapData = {
        "                                                                        ",
        "                                                                        ",
        "                                                                        ",
        "                    111                                                 ",
        "                                     111                                ",
        "           111                  111       111                           ",
        "                                               111                      ",
        "111111111111111111111111111111111111111111111111111111111111111111111111",
        "222222222222222222222222222222222222222222222222222222222222222222222222",
        "222222222      22222        222222222       2222222             22222222"
    };
}

std::vector<sf::FloatRect> LevelModel::getNearbyWalls(float x, float y) const {
    std::vector<sf::FloatRect> walls;
    float finalSize = m_tileSize * m_scale;

    // Convertir la position pixels en index grille
    int gridX = (int)(x / finalSize);
    int gridY = (int)(y / finalSize);

    // On vérifie les cases autour du joueur (3x3 cases)
    for (int i = gridY - 2; i <= gridY + 2; i++) {
        for (int j = gridX - 2; j <= gridX + 2; j++) {
            if (i >= 0 && i < m_mapData.size() && j >= 0 && j < m_mapData[i].size()) {
                char cell = m_mapData[i][j];
                // Si c'est '1' ou '2', c'est solide
                if (cell == '1' || cell == '2') {
                    walls.push_back(sf::FloatRect(j * finalSize, i * finalSize, finalSize, finalSize));
                }
            }
        }
    }
    return walls;
}

const std::vector<std::string>& LevelModel::getMapData() const { return m_mapData; }
float LevelModel::getTileSize() const { return m_tileSize * m_scale; }
