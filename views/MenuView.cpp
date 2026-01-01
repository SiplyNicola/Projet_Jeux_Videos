#include "MenuView.h"
#include <cmath> // For pulse animation logic

MenuView::MenuView() {}

bool MenuView::init(sf::RenderWindow& window) {
    // 1. Load background resources
    if (!m_backgroundTexture.loadFromFile("resources/MainMenu.png")) return false;
    m_backgroundSprite.setTexture(m_backgroundTexture);

    // Darken background for better text readability
    m_backgroundSprite.setColor(sf::Color(150, 150, 150));

    // Scale background to fit current window size
    sf::Vector2u m_winSize = window.getSize();
    float m_scaleX = (float)m_winSize.x / m_backgroundTexture.getSize().x;
    float m_scaleY = (float)m_winSize.y / m_backgroundTexture.getSize().y;
    m_backgroundSprite.setScale(m_scaleX, m_scaleY);

    // 2. Load Font
    if (!m_font.loadFromFile("resources/fonts/PressStart2P-regular.ttf")) return false;

    // 3. Initialize MAIN MENU Buttons
    std::vector<std::string> m_labels = { "START GAME", "SETTINGS", "EXIT" };
    for (int i = 0; i < (int)m_labels.size(); ++i) {
        sf::Text m_text;
        m_text.setFont(m_font);
        m_text.setString(m_labels[i]);
        m_text.setCharacterSize(35);
        m_text.setFillColor(sf::Color::Yellow);
        m_text.setOutlineColor(sf::Color::Red);
        m_text.setOutlineThickness(3.0f);

        // Center origin for rotation/scaling/positioning
        sf::FloatRect m_bounds = m_text.getLocalBounds();
        m_text.setOrigin(m_bounds.left + m_bounds.width / 2.0f, m_bounds.top + m_bounds.height / 2.0f);
        m_text.setPosition(m_winSize.x / 2.0f, m_winSize.y * 0.60f + (i * 90.0f));

        m_optionsTexts.push_back(m_text);
    }

    // 4. Initialize SETTINGS Text (Game Controls)
    std::vector<std::string> m_controlsList = {
        "CONTROLS",
        "Move Right : D",
        "Move Left  : Q",
        "Jump       : Z",
        "Dash       : J",
        "Attack     : K",
        "",
        "Press ESC to go back"
    };

    for (int i = 0; i < (int)m_controlsList.size(); ++i) {
        sf::Text m_text;
        m_text.setFont(m_font);
        m_text.setString(m_controlsList[i]);
        m_text.setCharacterSize(i == 0 ? 40 : 20); // Title is larger than keys
        m_text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::Yellow);

        // Horizontal centering
        sf::FloatRect m_bounds = m_text.getLocalBounds();
        m_text.setOrigin(m_bounds.left + m_bounds.width / 2.0f, m_bounds.top + m_bounds.height / 2.0f);
        m_text.setPosition(m_winSize.x / 2.0f, 150.0f + (i * 60.0f));

        m_settingsTexts.push_back(m_text);
    }

    return true;
}

void MenuView::draw(sf::RenderWindow& window, const MenuModel& m_model) {
    window.draw(m_backgroundSprite);

    // Check which screen should be rendered based on the Model state
    if (m_model.getState() == MenuState::MAIN) {
        // --- Render Main Menu ---
        float m_time = m_animationClock.getElapsedTime().asSeconds();
        float m_pulse = 1.0f + std::sin(m_time * 6.0f) * 0.08f;

        for (int i = 0; i < (int)m_optionsTexts.size(); ++i) {
            if (i == m_model.getSelectedIndex()) {
                // Highlighted button styling
                m_optionsTexts[i].setScale(m_pulse * 1.2f, m_pulse * 1.2f);
                m_optionsTexts[i].setFillColor(sf::Color(255, 255, 200));

                std::string m_base = (i == 0 ? "START GAME" : (i == 1 ? "SETTINGS" : "EXIT"));
                m_optionsTexts[i].setString("> " + m_base + " <");
            } else {
                // Normal button styling
                m_optionsTexts[i].setScale(1.0f, 1.0f);
                m_optionsTexts[i].setFillColor(sf::Color::Yellow);
                std::string m_base = (i == 0 ? "START GAME" : (i == 1 ? "SETTINGS" : "EXIT"));
                m_optionsTexts[i].setString(m_base);
            }

            // Re-center origin because text length changes (arrows)
            sf::FloatRect m_bounds = m_optionsTexts[i].getLocalBounds();
            m_optionsTexts[i].setOrigin(m_bounds.left + m_bounds.width / 2.0f, m_bounds.top + m_bounds.height / 2.0f);
            window.draw(m_optionsTexts[i]);
        }
    }
    else {
        // --- Render Settings Screen ---
        for (const auto& m_text : m_settingsTexts) {
            window.draw(m_text);
        }
    }
}
