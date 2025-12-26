#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "MenuModel.h"

class MenuView {
public:
    MenuView();
    bool init(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, const MenuModel& m_model);

    // Accessor for the controller to perform AABB collision checks
    const std::vector<sf::Text>& getButtons() const { return m_optionsTexts; }

private:
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::Font m_font;
    std::vector<sf::Text> m_optionsTexts;
};

#endif
