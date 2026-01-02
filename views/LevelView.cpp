#include "LevelView.h"
#include <iostream>

// Helper pour charger et vérifier les erreurs
void LevelView::loadTextureProp(const std::string& name, const std::string& path) {
    if (!m_textures[name].loadFromFile(path)) {
        std::cerr << "ERREUR : Impossible de charger la decoration : " << path << std::endl;
    }
}

void LevelView::init() {
    // 1. Tileset Terrain (comme avant)
    if(!m_tileset.loadFromFile("resources/oak_woods_v1.0/oak_woods_tileset.png")) {
        std::cerr << "ERREUR CRITIQUE : oak_woods_tileset.png introuvable !" << std::endl;
    }

    // 2. Chargement des Décorations (selon ton screenshot)
    std::string path = "resources/oak_woods_v1.0/decorations/";

    loadTextureProp("shop",   path + "shop.png");
    loadTextureProp("fence",  path + "fence_1.png"); // J'ai pris fence_1
    loadTextureProp("grass",  path + "grass_1.png"); // J'ai pris grass_1
    loadTextureProp("lamp",   path + "lamp.png");
    loadTextureProp("rock",   path + "rock_1.png");  // J'ai pris rock_1
    loadTextureProp("sign",   path + "sign.png");
    loadTextureProp("bridge", "resources/bridge/bridge.png");
}

void LevelView::build(const LevelModel& model) {
    const auto& map = model.getMapData();
    if (map.empty()) return;

    int height = map.size();
    int width = map[0].size();
    float ts = model.getTileSize(); // Sera 24 ou 16 selon le niveau
    int levelId = model.getLevelId(); // On récupère l'ID

    // Reset complet
    m_vertices.clear();
    m_decorations.clear();

    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            char cell = map[y][x];

            // --- GESTION DES DECORATIONS (SPRITES) ---
            sf::Texture* currentTex = nullptr;

            // Mapping Lettre -> Texture
            if (cell == 'S') currentTex = &m_textures["shop"];
            else if (cell == 'P') currentTex = &m_textures["sign"];
            else if (cell == 'L') currentTex = &m_textures["lamp"];
            else if (cell == 'F') currentTex = &m_textures["fence"];
            else if (cell == 'G') currentTex = &m_textures["grass"];
            else if (cell == 'R') currentTex = &m_textures["rock"];
            else if (cell == 'B') currentTex = &m_textures["bridge"];

            if (currentTex != nullptr) {
                // Création du sprite
                sf::Sprite s;
                s.setTexture(*currentTex);

                // CALCUL DE POSITION INTELLIGENT
                // On aligne le BAS de l'image avec le BAS de la case (pour que ce soit posé au sol)
                // Formule : y * taille_case + taille_case - hauteur_image
                float posX = x * ts;
                float posY = (y * ts) + ts - currentTex->getSize().y;

                // Petit ajustement pour centrer les objets fins (Lampes, Panneaux)
                if (cell == 'L' || cell == 'P' || cell == 'R' || cell == 'G') {
                     posX += (ts - currentTex->getSize().x) / 2.0f;
                }

                // CENTRAGE DU PONT ('B')
                // Si l'image est plus large que la case (ex: 96px vs 24px), on la centre.
                if (cell == 'B') {
                    // Formule : (Position X case) - (Moitié largeur Image) + (Moitié largeur Case)
                    posX = (x * ts) - (currentTex->getSize().x / 2.0f) + (ts / 2.0f);

                    // Ajustement vertical si besoin (décommente pour tester)
                    // posY += 5.0f;
                }

                // Le Shop est très grand, on le laisse s'étendre, mais on le décale un peu à gauche si besoin
                // (Là je le laisse par défaut à la case X)

                s.setPosition(posX, posY);
                m_decorations.push_back(s);
            }

            // --- GESTION DU TERRAIN (VERTEX ARRAY) ---
            int index = (x + y * width) * 6;
            sf::Vertex* tri = &m_vertices[index];

            // Si c'est vide OU une décoration, on ne dessine pas de terrain (transparent)
            if (cell == ' ' || cell == 'b' || currentTex != nullptr) {
                int index = (x + y * width) * 6;
                for(int i=0; i<6; ++i) {
                    m_vertices[index + i].position = sf::Vector2f(0,0);
                    m_vertices[index + i].color = sf::Color::Transparent;
                }
                continue;
            }
            // Si on est ici, c'est que cell est '1', '2', '3' ou '4'
            float px = x * ts;
            float py = y * ts;

            // Géométrie (Carré)
            tri[0].position = sf::Vector2f(px, py);
            tri[1].position = sf::Vector2f(px + ts, py);
            tri[2].position = sf::Vector2f(px, py + ts);
            tri[3].position = sf::Vector2f(px, py + ts);
            tri[4].position = sf::Vector2f(px + ts, py);
            tri[5].position = sf::Vector2f(px + ts, py + ts);

            for(int i=0; i<6; ++i) tri[i].color = sf::Color::White;

            // Coordonnées UV (Texture Terrain)
            int tu = 0, tv = 0;
            if (levelId == 1){
                if (cell == '1')      { tu = 1; tv = 0; } // Herbe
                else if (cell == '2') { tu = 1; tv = 1; } // Terre
                else if (cell == '3') { tu = 0; tv = 1; } // Bord G
                else if (cell == '4') { tu = 3; tv = 1; } // Bord D
            }

            else if (levelId == 2) {
                // --- LOGIQUE GROTTE (16x16) ---
                // OUVRE TON FICHIER PNG GROTTE ET COMPTE LES CASES (commence à 0)
                // Exemple hypothétique :
                if (cell == '1')      { tu = 1; tv = 0; } // Sol centre
                else if (cell == '2') { tu = 2; tv = 1; } // Mur gauche
                else if (cell == '3') { tu = 0; tv = 1; } // mur droit
                else if (cell == '4') { tu = 1; tv = 2; } // plafond
                else if (cell == '5') { tu = 3; tv = 0; } // coin haut gauche
                else if (cell == '6') { tu = 4; tv = 0; } // coin haut droit
                else if (cell == '7') { tu = 3; tv = 1; } // coin bas gauche
                else if (cell == '8') { tu = 4; tv = 1; } // coin bas droit
            }

            float u = tu * ts;
            float v = tv * ts;

            tri[0].texCoords = sf::Vector2f(u, v);
            tri[1].texCoords = sf::Vector2f(u + ts, v);
            tri[2].texCoords = sf::Vector2f(u, v + ts);
            tri[3].texCoords = sf::Vector2f(u, v + ts);
            tri[4].texCoords = sf::Vector2f(u + ts, v);
            tri[5].texCoords = sf::Vector2f(u + ts, v + ts);
        }
    }

    // APPLICATION DU ZOOM
    this->setScale(model.getScale(), model.getScale());
}

void LevelView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); // Zoom global

    // 1. D'abord le terrain
    states.texture = &m_tileset;
    target.draw(m_vertices, states);

    // 2. Ensuite les décorations (par dessus)
    for (const auto& deco : m_decorations) {
        target.draw(deco, states);
    }
}

void LevelView::loadTileset(const std::string& path) {
    if (!m_tileset.loadFromFile(path)) {
        std::cerr << "ERREUR : Impossible de charger le tileset " << path << std::endl;
    }
}
