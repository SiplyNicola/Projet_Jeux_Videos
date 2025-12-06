#pragma once
#include <SFML/Graphics.hpp>
#include "LevelModel.h"
#include <vector>
#include <map>
#include <string>

class LevelView : public sf::Transformable, public sf::Drawable {
private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    // --- NOUVEAU POUR LES DECOS ---
    std::map<std::string, sf::Texture> m_textures; // Stocke les images chargées
    std::vector<sf::Sprite> m_decorations;         // Liste des objets à afficher

    // Petite fonction pour charger proprement
    void loadTextureProp(const std::string& name, const std::string& path);

public:
    void init();
    void build(const LevelModel& model);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
