#pragma once
#include <SFML/Graphics.hpp>
#include "LevelModel.h"

// On hérite de Drawable (pour être dessiné) et Transformable (pour le zoom/position)
class LevelView : public sf::Drawable, public sf::Transformable {
public:
    void init();
    void build(const LevelModel& model);

private:
    // Méthode obligatoire de la classe sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray m_vertices; // Le tableau optimisé de triangles
    sf::Texture m_tileset;      // L'image unique
};
