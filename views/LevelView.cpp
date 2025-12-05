#include "LevelView.h"
#include <iostream>

void LevelView::init() {
    // Vérifie bien que l'image est dans le dossier !
    if(!m_tileset.loadFromFile("resources/oak_woods_v1.0/oak_woods_tileset.png")) {
        std::cerr << "ERREUR CRITIQUE : oak_woods_tileset.png introuvable !" << std::endl;
    }
}

void LevelView::build(const LevelModel& model) {
    const auto& map = model.getMapData();

    int height = map.size();
    int width = map[0].size();
    float ts = model.getTileSize(); // 24.0f

    // 1. Configuration VertexArray en mode TRIANGLES
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6); // 6 points par tuile (2 triangles)

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            char cell = map[y][x];
            int index = (x + y * width) * 6;
            sf::Vertex* tri = &m_vertices[index];

            // Si vide -> Transparent (on voit le background)
            if (cell == ' ') {
                for(int i=0; i<6; ++i) {
                    tri[i].position = sf::Vector2f(0,0);
                    tri[i].color = sf::Color::Transparent;
                }
                continue;
            }

            // A. Position Géométrique (Carré de 24x24)
            float px = x * ts;
            float py = y * ts;

            // Triangle 1
            tri[0].position = sf::Vector2f(px, py);
            tri[1].position = sf::Vector2f(px + ts, py);
            tri[2].position = sf::Vector2f(px, py + ts);
            // Triangle 2
            tri[3].position = sf::Vector2f(px, py + ts);
            tri[4].position = sf::Vector2f(px + ts, py);
            tri[5].position = sf::Vector2f(px + ts, py + ts);

            // Couleur blanche (opaque)
            for(int i=0; i<6; ++i) tri[i].color = sf::Color::White;

            // B. Coordonnées de Texture (UV) dans le PNG
            int tu = 0, tv = 0;
            // Mapping (selon oak_woods_tileset.png)
            if (cell == '1')      { tu = 1; tv = 0; } // Herbe
            else if (cell == '2') { tu = 1; tv = 1; } // Terre
            else if (cell == '3') { tu = 0; tv = 1; } // Bord Gauche
            else if (cell == '4') { tu = 2; tv = 1; } // Bord Droit

            float u = tu * ts;
            float v = tv * ts;

            // Mapping UV
            tri[0].texCoords = sf::Vector2f(u, v);
            tri[1].texCoords = sf::Vector2f(u + ts, v);
            tri[2].texCoords = sf::Vector2f(u, v + ts);

            tri[3].texCoords = sf::Vector2f(u, v + ts);
            tri[4].texCoords = sf::Vector2f(u + ts, v);
            tri[5].texCoords = sf::Vector2f(u + ts, v + ts);
        }
    }

    // APPLICATION DU ZOOM X3
    this->setScale(model.getScale(), model.getScale());
}

void LevelView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); // Applique le zoom
    states.texture = &m_tileset;        // Applique l'image
    target.draw(m_vertices, states);    // Dessine tout
}
