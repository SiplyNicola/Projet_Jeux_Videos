#include "MenuController.h"
#include <cmath>

void MenuController::run(sf::RenderWindow& window) {
    // Reset the view to the default for the menu UI
    window.setView(window.getDefaultView());
    MenuModel m_menuModel;
    MenuView m_menuView;

    // Load textures and fonts; exit if resources are missing
    if (!m_menuView.init(window)) return;

    // --- MAIN MENU LOOP ---
    // Continues until the window is closed or the game starts
    while (window.isOpen() && !m_menuModel.isGameStarted()) {
        sf::Event m_event;
        while (window.pollEvent(m_event)) {
            if (m_event.type == sf::Event::Closed) {
                window.close();
            }
            // Delegate user input to the handler
            handleInput(window, m_event, m_menuModel, m_menuView);
        }

        // Rendering phase
        window.clear();
        m_menuView.draw(window, m_menuModel);
        window.display();
    }
}

void MenuController::handleInput(sf::RenderWindow& window, sf::Event& event, MenuModel& model, const MenuView& view) {

    // --- SETTINGS STATE LOGIC ---
    // If the user is viewing controls, we only listen for the back command
    if (model.getState() == MenuState::SETTINGS) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            model.setState(MenuState::MAIN);
        }
        return; // Skip the rest of the menu button logic
    }

    // --- MAIN MENU STATE LOGIC ---
    sf::Vector2i m_mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f m_mouseCoords = window.mapPixelToCoords(m_mousePos);
    const std::vector<sf::Text>& m_buttons = view.getButtons();

    // 1. MOUSE INTERACTION: Check collisions with menu buttons
    for (int i = 0; i < (int)m_buttons.size(); ++i) {
        if (m_buttons[i].getGlobalBounds().contains(m_mouseCoords)) {
            // Update selection index on hover
            if (model.getSelectedIndex() != i) {
                model.setSelectedIndex(i);
            }

            // Handle Left Click triggers
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (i == 0) model.startGame();                  // "START GAME"
                if (i == 1) model.setState(MenuState::SETTINGS); // "SETTINGS"
                if (i == 2) window.close();                     // "EXIT"
            }
        }
    }

    // 2. KEYBOARD INTERACTION: Navigation and selection
    if (event.type == sf::Event::KeyPressed) {
        // Navigation using Z/S or Arrows
        if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up) model.previousOption();
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) model.nextOption();

        // Execute selected option
        else if (event.key.code == sf::Keyboard::Enter) {
            int m_idx = model.getSelectedIndex();
            if (m_idx == 0) model.startGame();
            if (m_idx == 1) model.setState(MenuState::SETTINGS);
            if (m_idx == 2) window.close();
        }
    }
}
