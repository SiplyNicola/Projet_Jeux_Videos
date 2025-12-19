#ifndef HUDVIEW_H
#define HUDVIEW_H

#include <SFML/Graphics.hpp>
#include <vector>

class HudView {
public:
    HudView();
    void init();
    // Affiche le nombre de cœurs correspondant à l'entier HP
    void draw(sf::RenderWindow& window, int currentHearts);

private:
    sf::Texture m_texture;
    sf::Sprite m_fullHeart;
    sf::Sprite m_emptyHeart;
    const int MAX_HEARTS = 5;
};

#endif
