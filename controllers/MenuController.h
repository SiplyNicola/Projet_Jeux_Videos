#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "MenuModel.h"

// Forward declaration to avoid circular inclusion
class MenuView;

class MenuController {
public:
    // Handles mouse and keyboard inputs for the menu
    void handleInput(sf::RenderWindow& window, sf::Event& event, MenuModel& model, const MenuView& view);
};

#endif
