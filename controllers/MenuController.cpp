#include "MenuController.h"
#include "MenuView.h" // Needed to access getButtons()

void MenuController::handleInput(sf::RenderWindow& window, sf::Event& event, MenuModel& model, const MenuView& view) {
    // Get mouse position relative to the current window view
    sf::Vector2i m_mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f m_mouseCoords = window.mapPixelToCoords(m_mousePos);

    const std::vector<sf::Text>& m_buttons = view.getButtons();

    // 1. MOUSE INTERACTION: AABB Collision Detection
    for (int i = 0; i < (int)m_buttons.size(); ++i) {
        // Check if the mouse point is inside the button's bounding box (AABB)
        if (m_buttons[i].getGlobalBounds().contains(m_mouseCoords)) {
            // Update the Model selection so the View can update visual effects
            model.setSelectedIndex(i);

            // Handle Left Click
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (i == 0) model.startGame();
                if (i == 2) window.close();
            }
        }
    }

    // 2. KEYBOARD INTERACTION: Fallback navigation
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up) {
            model.previousOption();
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            model.nextOption();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            int m_idx = model.getSelectedIndex();
            if (m_idx == 0) model.startGame();
            if (m_idx == 2) window.close();
        }
    }
}
