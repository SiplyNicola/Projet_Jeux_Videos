#ifndef NPCVIEW_H
#define NPCVIEW_H

#include <SFML/Graphics.hpp>

class NpcView {
public:
    NpcView(); // Constructeur (optionnel, mais propre)

    void init();
    void draw(sf::RenderWindow& window, sf::Vector2f position);

private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
};

#endif
