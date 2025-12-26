#include <SFML/Graphics.hpp>
#include "MenuModel.h"
#include "MenuView.h"
#include "MenuController.h"
#include "Game.h"

int main() {
    sf::RenderWindow m_window(sf::VideoMode(1920, 1080), "Whisper of Steel");
    m_window.setFramerateLimit(60);

    MenuModel m_menuModel;
    MenuView m_menuView;
    MenuController m_menuController;

    // Call init with the window reference
    if (!m_menuView.init(m_window)) return -1;

    // --- MENU LOOP ---
    while (m_window.isOpen() && !m_menuModel.isGameStarted()) {
        sf::Event m_event;
        while (m_window.pollEvent(m_event)) {
            if (m_event.type == sf::Event::Closed) m_window.close();
            m_menuController.handleInput(m_window, m_event, m_menuModel, m_menuView);
        }

        m_window.clear();
        m_menuView.draw(m_window, m_menuModel);
        m_window.display();
    }

    // --- TRANSITION TO GAME ---
    if (m_menuModel.isGameStarted()) {
        // Create the game instance and run the main loop
        Game m_game;
        m_game.run();
    }

    return 0;
}
