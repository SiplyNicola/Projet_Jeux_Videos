#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <SFML/Graphics.hpp>
#include "MenuModel.h"
#include "MenuView.h"

class MenuController {
public:
    // Main method to launch and manage the entire menu loop
    void run(sf::RenderWindow& window);

    // Handles mouse and keyboard inputs for the menu
    void handleInput(sf::RenderWindow& window, sf::Event& event, MenuModel& model, const MenuView& view);
};

#endif
