#include "LevelModel.h"

LevelModel::LevelModel() {
    m_tileSize = 24.0f;
    m_scale = 3.0f; // 72px par bloc
}

void LevelModel::loadLevel() {
    // DESIGN "PLATFORMER RYTHMÉ"
    // On garde les étages, mais on ajoute des obstacles DANS les étages.

    // Hauteur : 35 lignes
    // Largeur : 130 blocs

    m_mapData = {
        // --- ETAGE 3 : VERS LA DROITE -> BOSS ---
        "                                                                                                                                  ",
        "                                                                                                         ==================       ",
        "                                                                                                        32222222222222222224      ",
        "                                                                                                        32222222222222222224      ",
        "                                                                                                        32224          32224      ",
        "                                                                                                        32224   BOSS   32224      ",
        "                           1111111                                                       11111111       32224          32224      ",
        "                             222234                  1111111       111111111                            32224          32224      ",
        "      1111111                                                                                           32224          32224      ",
        "                                            11111                        1111111                        32224          32224      ", // Entrée Arène
        "                111111        11111         3222224                                                     32224          32224      ",
        "                                                                                                        32224          32224      ",
        "                                 1111                          111111                                   32224          32224      ",
        "      111111                    322224                                                                  32224          32224      ",
        "                                                      111111                                            32224          32224      ",
        "                       1111                                                                             32224          32224      ",
        "                      322224                                                                            32224          32224      ",
        "            11111                                                                                                                  ",
        "                                                                                                                                  ",
        "    111111                                                                                                                        ",
        // Ici, j'ai ajouté des trous et des plateformes flottantes pour que ce ne soit pas juste un couloir
        "             1111111                     11111111                  11111                                                          ",
        "            322222224     1111111       3222222224                3222224                                                         ",
        "                                                                                                                                  ",
        "                                                       111111             1111                   11111             1111           ", // Plateformes volantes
        "                                                                         322224                                    322224         ",
        "                                                                                                         11111                    ",
        "                       111111                                                      111111                                         ",
        "                      32222224                                                    32222224                        1111            ",
        "                                                                                                          1111                    ", // Escalier de transition
        "                                                                                                         322224                   ",
        "                                                                                                                                  ",
        "                                                                                                1111                              ",
        "                                                                                               322224                             ",

        // --- ETAGE 1 : VERS LA DROITE (INITIATION AU SAUT) ---
        "                                                                                                                                  ",
        // Au lieu d'un sol plat, j'ai mis des "Haies" (piliers) à sauter et des trous
        "111111111111          11111111111               1111111111111111           111111111111111111111111111111111111111111111          ",
        "222222222222          22222222222               2222222222222222           222222222222222222222222222222222222222222222          ",
        "222222222222   1111   22222222222      1111     2222222222222222    1111   222222222222222222222222222222222222222222222          ",
        "222222222222  322224  22222222222     322224    2222222222222222   322224  222222222222222222222222222222222222222222222          "
    };
}

std::vector<sf::FloatRect> LevelModel::getNearbyWalls(float x, float y) const {
    std::vector<sf::FloatRect> walls;
    float finalSize = m_tileSize * m_scale;
    int gridX = (int)(x / finalSize);
    int gridY = (int)(y / finalSize);

    for (int i = gridY - 3; i <= gridY + 3; i++) {
        for (int j = gridX - 3; j <= gridX + 3; j++) {
            if (i >= 0 && i < m_mapData.size() && j >= 0 && j < m_mapData[i].size()) {
                char cell = m_mapData[i][j];
                // Tout ce qui n'est pas vide est solide
                if (cell != ' ') {
                    walls.push_back(sf::FloatRect(j * finalSize, i * finalSize, finalSize, finalSize));
                }
            }
        }
    }
    return walls;
}

const std::vector<std::string>& LevelModel::getMapData() const { return m_mapData; }
float LevelModel::getTileSize() const { return m_tileSize; }
float LevelModel::getScale() const { return m_scale; }
