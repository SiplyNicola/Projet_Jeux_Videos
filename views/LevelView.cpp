#include "LevelView.h"
#include <iostream>

/**
 * Helper function to load a texture and handle errors.
 * Stores the loaded texture in the m_textures map for easy access during the build process.
 * @param name The unique identifier for the decoration.
 * @param path The file system path to the image asset.
 */
void LevelView::loadTextureProp(const std::string& name, const std::string& path) {
    if (!m_textures[name].loadFromFile(path)) {
        std::cerr << "ERROR: Unable to load decoration: " << path << std::endl;
    }
}

/**
 * Initializes the visual assets of the level.
 * Loads the main terrain tileset and all individual decoration textures.
 */
void LevelView::init() {
    // 1. Terrain Tileset Initialization
    // This sheet contains the basic building blocks like grass, dirt, and wall tiles.
    if(!m_tileset.loadFromFile("resources/oak_woods_v1.0/oak_woods_tileset.png")) {
        std::cerr << "CRITICAL ERROR: oak_woods_tileset.png not found!" << std::endl;
    }

    // 2. Loading Decorations (Sprites)
    // These are standalone images that are layered on top of the terrain grid.
    std::string path = "resources/oak_woods_v1.0/decorations/";

    loadTextureProp("shop",   path + "shop.png");
    loadTextureProp("fence",  path + "fence_1.png");
    loadTextureProp("grass",  path + "grass_1.png");
    loadTextureProp("lamp",   path + "lamp.png");
    loadTextureProp("rock",   path + "rock_1.png");
    loadTextureProp("sign",   path + "sign.png");
    loadTextureProp("bridge", "resources/bridge/bridge.png");
}

/**
 * Builds the graphical representation of the level.
 * Generates a VertexArray for the terrain and populates a list of sprites for decorations.
 * @param model Reference to the LevelModel containing the ASCII map and tile data.
 */
void LevelView::build(const LevelModel& model) {
    const auto& map = model.getMapData();
    if (map.empty()) return;

    int height = map.size();
    int width = map[0].size();
    float ts = model.getTileSize();   // Tile size varies (24 or 16) based on level ID
    int levelId = model.getLevelId(); // Identify the current level logic

    // Complete reset of graphical data before rebuilding
    m_vertices.clear();
    m_decorations.clear();

    // Initialize the terrain vertex array using triangles (6 vertices per square tile)
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            char cell = map[y][x];

            // --- DECORATION MANAGEMENT (SPRITES) ---
            // Map the ASCII character to a pre-loaded texture pointer
            sf::Texture* currentTex = nullptr;

            if (cell == 'S')      currentTex = &m_textures["shop"];
            else if (cell == 'P') currentTex = &m_textures["sign"];
            else if (cell == 'L') currentTex = &m_textures["lamp"];
            else if (cell == 'F') currentTex = &m_textures["fence"];
            else if (cell == 'G') currentTex = &m_textures["grass"];
            else if (cell == 'R') currentTex = &m_textures["rock"];
            else if (cell == 'B') currentTex = &m_textures["bridge"];

            if (currentTex != nullptr) {
                sf::Sprite s;
                s.setTexture(*currentTex);

                // INTELLIGENT POSITIONING
                // Align the BOTTOM of the image with the BOTTOM of the tile grid square.
                // Formula: (y * tile_size) + tile_size - image_height
                float posX = x * ts;
                float posY = (y * ts) + ts - currentTex->getSize().y;

                // Minor adjustment to center thin objects (Lamps, Signs, Rocks, Grass)
                if (cell == 'L' || cell == 'P' || cell == 'R' || cell == 'G') {
                     posX += (ts - currentTex->getSize().x) / 2.0f;
                }

                // BRIDGE CENTERING ('B')
                // If the bridge image is wider than the tile (e.g., 96px vs 24px), we center it on the cell.
                if (cell == 'B') {
                    // Formula: (Cell X position) - (Half Image Width) + (Half Tile Width)
                    posX = (x * ts) - (currentTex->getSize().x / 2.0f) + (ts / 2.0f);
                }

                s.setPosition(posX, posY);
                m_decorations.push_back(s);
            }

            // --- TERRAIN MANAGEMENT (VERTEX ARRAY) ---
            int index = (x + y * width) * 6;
            sf::Vertex* tri = &m_vertices[index];

            // If the cell is empty or contains a decoration, we render a transparent tile.
            if (cell == ' ' || cell == 'b' || currentTex != nullptr) {
                for(int i=0; i<6; ++i) {
                    m_vertices[index + i].position = sf::Vector2f(0,0);
                    m_vertices[index + i].color = sf::Color::Transparent;
                }
                continue;
            }

            // Define geometry for numeric terrain tiles (1, 2, 3, 4, etc.)
            float px = x * ts;
            float py = y * ts;

            // Square geometry formed by two triangles
            tri[0].position = sf::Vector2f(px, py);
            tri[1].position = sf::Vector2f(px + ts, py);
            tri[2].position = sf::Vector2f(px, py + ts);
            tri[3].position = sf::Vector2f(px, py + ts);
            tri[4].position = sf::Vector2f(px + ts, py);
            tri[5].position = sf::Vector2f(px + ts, py + ts);

            for(int i=0; i<6; ++i) tri[i].color = sf::Color::White;

            // UV Texture Coordinates Logic
            int tu = 0, tv = 0;
            if (levelId == 1){
                // Mapping for Level 1 (Forest)
                if (cell == '1')      { tu = 1; tv = 0; } // Top Grass
                else if (cell == '2') { tu = 1; tv = 1; } // Underground Dirt
                else if (cell == '3') { tu = 0; tv = 1; } // Left Dirt Edge
                else if (cell == '4') { tu = 3; tv = 1; } // Right Dirt Edge
            }
            else if (levelId == 2) {
                // Mapping for Level 2 (Cave)
                if (cell == '1')      { tu = 1; tv = 0; } // Floor center
                else if (cell == '2') { tu = 2; tv = 1; } // Left Wall
                else if (cell == '3') { tu = 0; tv = 1; } // Right Wall
                else if (cell == '4') { tu = 1; tv = 2; } // Ceiling
                else if (cell == '5') { tu = 3; tv = 0; } // Top-Left Corner
                else if (cell == '6') { tu = 4; tv = 0; } // Top-Right Corner
                else if (cell == '7') { tu = 3; tv = 1; } // Bottom-Left Corner
                else if (cell == '8') { tu = 4; tv = 1; } // Bottom-Right Corner
                else if (cell == 'v') { tu = 1; tv = 1; } // nothing
                else if (cell == 'a') { tu = 0; tv = 0; } // inner corner
                else if (cell == 'z') { tu = 2; tv = 0; } // inner corner
                else if (cell == 'q') { tu = 0; tv = 2; } // inner corner
                else if (cell == 's') { tu = 2; tv = 2; } // inner corner
            }

            float u = tu * ts;
            float v = tv * ts;

            // Assign UV coordinates to vertices
            tri[0].texCoords = sf::Vector2f(u, v);
            tri[1].texCoords = sf::Vector2f(u + ts, v);
            tri[2].texCoords = sf::Vector2f(u, v + ts);
            tri[3].texCoords = sf::Vector2f(u, v + ts);
            tri[4].texCoords = sf::Vector2f(u + ts, v);
            tri[5].texCoords = sf::Vector2f(u + ts, v + ts);
        }
    }

    // Apply global level scaling to match the physical tile size (e.g., scale 3.0f for 72px tiles)
    this->setScale(model.getScale(), model.getScale());
}

/**
 * Custom draw function to render the level.
 * Inherits the global view transformation for zoom and positioning.
 */
void LevelView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); // Apply global zoom/scaling

    // 1. Render the Terrain Vertex Array first (lowest layer)
    states.texture = &m_tileset;
    target.draw(m_vertices, states);

    // 2. Render Decorations on top of the terrain
    for (const auto& deco : m_decorations) {
        target.draw(deco, states);
    }
}

/**
 * Loads a specific tileset image for the terrain.
 * Used during level transitions (e.g., Forest to Cave).
 * @param path The file path to the new tileset PNG.
 */
void LevelView::loadTileset(const std::string& path) {
    if (!m_tileset.loadFromFile(path)) {
        std::cerr << "ERROR: Unable to load tileset " << path << std::endl;
    }
}
