#include "MenuController.h"
#include <cmath> // For animation logic

void MenuController::run(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    MenuModel m_menuModel;
    MenuView m_menuView;

    // Initialize view resources (textures, fonts, buttons)
    if (!m_menuView.init(window)) return;

    // --- MENU LOOP (Encapsulated) ---
    // Continues as long as the window is open and game hasn't started
    while (window.isOpen() && !m_menuModel.isGameStarted()) {
        sf::Event m_event;
        while (window.pollEvent(m_event)) {
            if (m_event.type == sf::Event::Closed) {
                window.close();
            }
            // Delegate input processing to handleInput
            handleInput(window, m_event, m_menuModel, m_menuView);
        }

        // Rendering phase using the MenuView drawing logic
        window.clear();
        m_menuView.draw(window, m_menuModel);
        window.display();
    }
}

void MenuController::handleInput(sf::RenderWindow& window, sf::Event& event, MenuModel& model, const MenuView& view) {
    sf::Vector2i m_mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f m_mouseCoords = window.mapPixelToCoords(m_mousePos);
    const std::vector<sf::Text>& m_buttons = view.getButtons(); //

    // 1. MOUSE INTERACTION: AABB Collision Detection
    for (int i = 0; i < (int)m_buttons.size(); ++i) {
        if (m_buttons[i].getGlobalBounds().contains(m_mouseCoords)) {
            // Update the selection index in the model
            if (model.getSelectedIndex() != i) {
                model.setSelectedIndex(i);
            }

            // Handle Left Click triggers
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (i == 0) model.startGame(); // Changes state to start game
                if (i == 2) window.close();
            }
        }
    }

    // 2. KEYBOARD INTERACTION: Navigation fallback
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up) model.previousOption();
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) model.nextOption();
        else if (event.key.code == sf::Keyboard::Enter) {
            int m_idx = model.getSelectedIndex();
            if (m_idx == 0) model.startGame();
            if (m_idx == 2) window.close();
        }
    }
}
