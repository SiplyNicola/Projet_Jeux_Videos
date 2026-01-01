#include "PauseController.h"

// --- DEFINITIONS TO FIX LINKER ERRORS ---

PauseController::PauseController() {}

PauseController::~PauseController() {}

PauseController::PauseController(const PauseController& m_other) {
    *this = m_other;
}

PauseController& PauseController::operator=(const PauseController& m_other) {
    if (this != &m_other) {
        // Copy members if any are added later
    }
    return *this;
}

void PauseController::handleInput(sf::RenderWindow& m_window, sf::Event& m_event, PauseModel& m_model, const PauseView& m_view) {
    // 1. SETTINGS SCREEN NAVIGATION
    if (m_model.getState() == PauseState::SETTINGS) {
        if (m_event.type == sf::Event::KeyPressed && m_event.key.code == sf::Keyboard::Escape) {
            m_model.setState(PauseState::MAIN);
        }
        return;
    }

    // 2. MOUSE INTERACTION
    sf::Vector2f m_mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    const std::vector<sf::Text>& m_buttons = m_view.getButtons();

    for (int i = 0; i < (int)m_buttons.size(); ++i) {
        if (m_buttons[i].getGlobalBounds().contains(m_mousePos)) {
            m_model.setSelectedIndex(i);
            if (m_event.type == sf::Event::MouseButtonReleased && m_event.mouseButton.button == sf::Mouse::Left) {
                if (i == 0) m_model.setResume(true);
                if (i == 1) m_model.setState(PauseState::SETTINGS);
                if (i == 2) m_model.setExitToMain(true);
                if (i == 3) m_window.close();
            }
        }
    }

    // 3. KEYBOARD NAVIGATION
    if (m_event.type == sf::Event::KeyPressed) {
        if (m_event.key.code == sf::Keyboard::Z || m_event.key.code == sf::Keyboard::Up) m_model.previousOption();
        else if (m_event.key.code == sf::Keyboard::S || m_event.key.code == sf::Keyboard::Down) m_model.nextOption();
        else if (m_event.key.code == sf::Keyboard::Enter) {
            int m_idx = m_model.getSelectedIndex();
            if (m_idx == 0) m_model.setResume(true);
            if (m_idx == 1) m_model.setState(PauseState::SETTINGS);
            if (m_idx == 2) m_model.setExitToMain(true);
            if (m_idx == 3) m_window.close();
        }
    }
}
