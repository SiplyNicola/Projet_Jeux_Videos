#include "LevelModel.h"

/**
 * LevelModel Constructor
 * Initializes default tile size and scale values used for world generation.
 */
LevelModel::LevelModel() {
    m_tileSize = 24.0f; // Default base tile size in pixels
    m_scale = 3.0f;     // Default rendering scale
}

/**
 * Loads the level data based on the provided ID.
 * Defines tile sizes, scales, and the ASCII map layout for different environments.
 * @param levelId The unique identifier of the level to load.
 */
void LevelModel::loadLevel(int levelId) {

    m_levelId = levelId; // Store the current level ID
    m_mapData.clear();    // Clear existing map data before loading new level

    // --- LEVEL 1: THE FOREST ---
    if (levelId == 1) {
        m_tileSize = 24.0f;
        m_scale = 3.0f;     // Final block size = 72px (24 * 3)

        // RHYTHMIC PLATFORMER DESIGN
        // Mapping Codes: S=Shop, P=Sign, L=Lamp, F=Fence, G=Grass, R=Rock, bBb=Bridge Point
        m_mapData = {
            // --- FLOOR 3: RIGHTWARD PATH TOWARDS BOSS ---
            "                                                                                                                                  ",
            "                                                                                                         ==================       ",
            "                                                                                                                                  ", // Shop placed here
            "                                                                                                                                  ",
            "                                                                                                                                  ",
            "                                                                                                                                  ", // Grass
            "                                                        P            L  F                  11bBbbBbbBbbBb                                       ",
            "      L F   L                                        1111111       111111111            11124                                     ",
            "      1111111                      1111             322222224                   L      322224                                   ",
            "                                  322224    GG                               1111111                                              ",
            "                111111    11111            111111                       1111                                                      ",
            "                                          32222224                     322224                                                     ",
            "                                 1111                          111111                                                             ",
            "                                322224                 G  R                                                                       ",
            "                         G                            111111                                                                      ",
            "                       1111                                                                                                       ",
            "              L F     322224                                                                                                      ",
            "             111111                                                                                                               ",
            "            32                                                                                                                    ",
            "                                           F G L                                                                                  ",
            "    111111     L F         G L G         11111111                  11111                                                          ",
            "             1111111      1111111       3222222224                3222224                                                         ",
            "            322222224                                     L                R                       P                              ",
            "                                                       111111             1111                   11111                            ",
            "                                                                         322224                            R                      ",
            "                                                                                     GR                  11111                    ",
            "                                                                                   111111                                         ",
            "                                                                                  32222224                  RG      1111          ",
            "                                                                                                          1111                    ",
            "                                                                                                         322224                   ",
            "                                                                                                 GG                               ",
            "                                                                                                1111                              ",
            "                                                                                               322224                             ",

            // --- FLOOR 1: RIGHTWARD PATH (JUMP INITIATION) ---
            "                                                                                              3                                   ", // Signs, Fences, Lamps
            " S        GG          FG  LG F  L                 F   L  L   F               G  L   F  G   L     GG   F    G       L              ",
            "111111111111          11111111111               1111111111111111           111111111111111111111111111111111111111111111          ",
            "222222222222          22222222222               2222222222222222           222222222222222222222222222222222222222222222          ",
            "222222222222   1111   22222222222      1111     2222222222222222    1111   222222222222222222222222222222222222222222222          ",
            "222222222222  322224  22222222222     322224    2222222222222222   322224  222222222222222222222222222222222222222222222          "
        };
    }
    // --- LEVEL 2: THE CAVE ---
    else if (levelId == 2) {
        m_tileSize = 16.0f; // Smaller base tile size for denser textures
        m_scale = 4.5f;     // Increased scale to maintain 72px physical size (16 * 4.5)

        // TUNNEL STYLE LAYOUT: A more enclosed, cave-like environment
        m_mapData = {
            "2  3vvvvvvvvvvvvvv",
            "2  3vvvv5444444446",
            "2  3vv54s        3",
            "2  q44s          3", // Platforms
            "2                3",
            "2                3",
            "2                3",
            "2      P         3",
            "711111111111111118", // Player spawn point
            "vvvvvvvvvvvvvvvvvv"
        };
    }
}

/**
 * Retrieves a list of wall hitboxes surrounding a specific world position.
 * Used to optimize collision detection by only checking nearby tiles.
 * @param x World X coordinate of the entity.
 * @param y World Y coordinate of the entity.
 * @return A vector of FloatRects representing solid wall boundaries.
 */
std::vector<sf::FloatRect> LevelModel::getNearbyWalls(float x, float y) const {
    std::vector<sf::FloatRect> walls;
    float finalSize = m_tileSize * m_scale;

    // Convert world coordinates to grid indices
    int gridX = (int)(x / finalSize);
    int gridY = (int)(y / finalSize);

    // Scan a 7x7 grid area around the entity (radius of 3 tiles)
    for (int i = gridY - 3; i <= gridY + 3; i++) {
        for (int j = gridX - 3; j <= gridX + 3; j++) {
            // Boundary check: ensure indices are within map limits
            if (i >= 0 && i < m_mapData.size() && j >= 0 && j < m_mapData[i].size()) {
                char cell = m_mapData[i][j];

                // SECURITY: Only numeric codes 1, 2, 3, and 4 are treated as SOLID WALLS.
                // All other characters (S, L, P, G, F, R, and empty spaces) are traversable.
                if (cell == '1' || cell == '2' || cell == '3' || cell == '4' ||
                    cell == '5' || cell == '6' || cell == '7' || cell == '8' ||
                    cell == 'a' || cell == 'z' || cell == 'q' || cell == 's')
                {
                    walls.push_back(sf::FloatRect(j * finalSize, i * finalSize, finalSize, finalSize));
                }
            }
        }
    }
    return walls;
}

/**
 * Accessor for the map's ASCII data.
 */
const std::vector<std::string>& LevelModel::getMapData() const { return m_mapData; }

/**
 * Accessor for the base tile size.
 */
float LevelModel::getTileSize() const { return m_tileSize; }

/**
 * Accessor for the visual scaling factor.
 */
float LevelModel::getScale() const { return m_scale; }
