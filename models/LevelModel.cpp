#include "LevelModel.h"

LevelModel::LevelModel() {
    m_tileSize = 24.0f;
    m_scale = 3.0f;
}

void LevelModel::loadLevel(int levelId) {

    m_levelId = levelId; // On mémorise le niveau
    m_mapData.clear();

    // --- NIVEAU 1 : LA FORÊT (Ton code actuel) ---
    if (levelId == 1) {
        m_tileSize = 24.0f;
        m_scale = 3.0f;     // Taille finale bloc = 72px
        // DESIGN "PLATFORMER RYTHMÉ" (Structure IDENTIQUE à ton fichier)
        // Ajout des codes : S=Shop, P=Panneau, L=Lampe, F=Fence, G=Grass, R=Rock

        m_mapData = {
            // --- ETAGE 3 : VERS LA DROITE -> BOSS ---
            "                                                                                                                                  ",
            "                                                                                                         ==================       ",
            "                                                                                                                                  ", // Le Shop posé ici
            "                                                                                                                                  ",
            "                                                                                                                                  ",
            "                                                                                                                                  ", // Herbe
            "                           1111111                      P            L  F                11111111                                 ",
            "      L F   L                222234                  1111111       111111111                                                      ",
            "      1111111                                                                   L                                                 ",
            "                                            GG     1111                      1111111                                              ",
            "                111111        11111        111111                                                                                 ",
            "                                          1111111                                                                                 ",
            "        RGT                      1111                          111111                                                             ",
            "      111111                    322224                 G  R                                                                       ",
            "                         G                            111111                                                                      ",
            "                       1111                                                                                                       ",
            "                      322224                                                                                                      ",
            "           11111                                                                                                                  ",
            "                                                                                                                                  ",
            "    111111     L F                         F G L                                                                                  ",
            "             1111111       G L G         11111111                  11111                                                          ",
            "            322222224     1111111       3222222224                3222224                                                         ",
            "                                                          L                R                       P                              ",
            "                                                       111111             1111                   11111             1111           ",
            "                                                                         322224                            R       322224         ",
            "                        L  F                                                         GR                  11111                    ",
            "                       111111                                                      111111                                         ",
            "                      32222224                                                    32222224                  RG      1111          ",
            "                                                                                                          1111                    ",
            "                                                                                                         322224                   ",
            "                                                                                                 GG                               ",
            "                                                                                                1111                              ",
            "                                                                                               322224                             ",

            // --- ETAGE 1 : VERS LA DROITE (INITIATION AU SAUT) ---
            "                                                                                              3                                   ", // Panneau, Barrières, Lampes
            " S        GG          FG  LG F  L                 F   L  L   F               G  L   F  G   L     GG   F    G       L              ",
            "111111111111          11111111111               1111111111111111           111111111111111111111111111111111111111111111          ",
            "222222222222          22222222222               2222222222222222           222222222222222222222222222222222222222222222          ",
            "222222222222   1111   22222222222      1111     2222222222222222    1111   222222222222222222222222222222222222222222222          ",
            "222222222222  322224  22222222222     322224    2222222222222222   322224  222222222222222222222222222222222222222222222          "
        };
    }
    // --- NIVEAU 2 : LA GROTTE (Nouvelle carte) ---
    else if (levelId == 2) {
        m_tileSize = 16.0f; // <--- CHANGEMENT ICI
        m_scale = 4.5f;     // <--- 16 * 4.5 = 72px (Même taille physique que la forêt)
        // Une map plus fermée, style tunnel
        m_mapData = {
            "54444444444444444444444444444444444444444444444446",
            "2                                                3",
            "2                                                3",
            "2      L       R               L                 3",
            "2    11111   11111           11111               3", // Plateformes
            "2                                                3",
            "2                                      R         3",
            "2                                    11111       3",
            "2      P                                         3",
            "2    11111                                       3", // Le joueur arrive ici
            "71111111111111111111111111111111111111111111111118"

        };
    }

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

                // SECURITE : On ne considère comme MUR que les chiffres 1, 2, 3, 4
                // Tout le reste (S, L, P, G, F, R et espace) est traversable
                if (cell == '1' || cell == '2' || cell == '3' || cell == '4') {
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
