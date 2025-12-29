#include "MenuView.h"
#include <cmath> // For sin() function (pulsation animation)

MenuView::MenuView() {}

bool MenuView::init(sf::RenderWindow& window) {
    // Load background image
    if (!m_backgroundTexture.loadFromFile("resources/MainMenu.png")) return false;
    m_backgroundSprite.setTexture(m_backgroundTexture);

    // Darken the background slightly to make the yellow text pop
    m_backgroundSprite.setColor(sf::Color(150, 150, 150));

    sf::Vector2u m_texSize = m_backgroundTexture.getSize();
    sf::Vector2u m_winSize = window.getSize();
    float m_scaleX = (float)m_winSize.x / m_texSize.x;
    float m_scaleY = (float)m_winSize.y / m_texSize.y;

    m_backgroundSprite.setScale(m_scaleX, m_scaleY);
    m_backgroundSprite.setPosition(0, 0);

    // Load Pixel Font
    if (!m_font.loadFromFile("resources/fonts/PressStart2P-regular.ttf")) return false;

    std::vector<std::string> m_labels = { "START GAME", "OPTIONS", "EXIT" };
    for (int i = 0; i < (int)m_labels.size(); ++i) {
        sf::Text m_text;
        m_text.setFont(m_font);
        m_text.setString(m_labels[i]);
        m_text.setCharacterSize(35);

        // --- REQUESTED STYLE: YELLOW WITH RED OUTLINE ---
        m_text.setFillColor(sf::Color::Yellow);
        m_text.setOutlineColor(sf::Color::Red);
        m_text.setOutlineThickness(3.0f); // Clearly visible outline

        // Center text origin
        sf::FloatRect m_textBounds = m_text.getLocalBounds();
        m_text.setOrigin(m_textBounds.left + m_textBounds.width / 2.0f,
                         m_textBounds.top + m_textBounds.height / 2.0f);

        // Vertical positioning
        m_text.setPosition(m_winSize.x / 2.0f, m_winSize.y * 0.60f + (i * 90.0f));
        m_optionsTexts.push_back(m_text);
    }
    return true;
}

void MenuView::draw(sf::RenderWindow& window, const MenuModel& m_model) {
    window.draw(m_backgroundSprite);

    // Calculate pulse factor based on time (oscillates between 1.0 and 1.15)
    float time = m_animationClock.getElapsedTime().asSeconds();
    float pulse = 1.0f + std::sin(time * 6.0f) * 0.08f;

    for (int i = 0; i < (int)m_optionsTexts.size(); ++i) {
        if (i == m_model.getSelectedIndex()) {
            // --- ANIMATION IF SELECTED ---
            m_optionsTexts[i].setScale(pulse * 1.2f, pulse * 1.2f); // Scaling up + Pulse effect
            m_optionsTexts[i].setOutlineColor(sf::Color(255, 50, 50)); // Brighter red
            m_optionsTexts[i].setFillColor(sf::Color(255, 255, 200));   // Lighter yellow (glow)

            // Add dynamic arrows to the selected text
            std::string originalStr = (i == 0 ? "START GAME" : (i == 1 ? "OPTIONS" : "EXIT"));
            m_optionsTexts[i].setString("> " + originalStr + " <");
        } else {
            // --- NORMAL STATE ---
            m_optionsTexts[i].setScale(1.0f, 1.0f);
            m_optionsTexts[i].setFillColor(sf::Color::Yellow);
            m_optionsTexts[i].setOutlineColor(sf::Color::Red);

            // Reset to original text without arrows
            std::string originalStr = (i == 0 ? "START GAME" : (i == 1 ? "OPTIONS" : "EXIT"));
            m_optionsTexts[i].setString(originalStr);
        }

        // Re-center origin in case the string length changed (due to arrows)
        sf::FloatRect m_textBounds = m_optionsTexts[i].getLocalBounds();
        m_optionsTexts[i].setOrigin(m_textBounds.left + m_textBounds.width / 2.0f,
                                    m_textBounds.top + m_textBounds.height / 2.0f);

        window.draw(m_optionsTexts[i]);
    }
}
