#include "MenuView.h"
#include <algorithm> // For std::min

MenuView::MenuView() {}

bool MenuView::init(sf::RenderWindow& window) {
    // Load background image
    if (!m_backgroundTexture.loadFromFile("resources/MainMenu.png")) return false;
    m_backgroundSprite.setTexture(m_backgroundTexture);

    // LESS DARK BACKGROUND: Dim the sprite color (180 is brighter than 100)
    m_backgroundSprite.setColor(sf::Color(180, 180, 180));

    // CENTER AND SCALE: Make sure the whole image fits the window
    sf::Vector2u m_texSize = m_backgroundTexture.getSize();
    sf::Vector2u m_winSize = window.getSize();
    float m_scaleX = (float)m_winSize.x / m_texSize.x;
    float m_scaleY = (float)m_winSize.y / m_texSize.y;

    // Use full window scaling to ensure visibility
    m_backgroundSprite.setScale(m_scaleX, m_scaleY);
    m_backgroundSprite.setPosition(0, 0);

    // Load Pixel Font
    if (!m_font.loadFromFile("resources/fonts/PressStart2P-regular.ttf")) return false;

    std::vector<std::string> m_labels = { "START GAME", "OPTIONS", "EXIT" };
    for (int i = 0; i < (int)m_labels.size(); ++i) {
        sf::Text m_text;
        m_text.setFont(m_font);
        m_text.setString(m_labels[i]);
        m_text.setCharacterSize(30);

        // Center text origin for perfect alignment
        sf::FloatRect m_textBounds = m_text.getLocalBounds();
        m_text.setOrigin(m_textBounds.left + m_textBounds.width / 2.0f,
                         m_textBounds.top + m_textBounds.height / 2.0f);

        // Position texts vertically in the center
        m_text.setPosition(m_winSize.x / 2.0f, m_winSize.y * 0.65f + (i * 80.0f));
        m_optionsTexts.push_back(m_text);
    }
    return true;
}

void MenuView::draw(sf::RenderWindow& window, const MenuModel& m_model) {
    window.draw(m_backgroundSprite);

    for (int i = 0; i < (int)m_optionsTexts.size(); ++i) {
        // Visual Feedback based on selection
        if (i == m_model.getSelectedIndex()) {
            // HOVER STATE: White color and bigger scale
            m_optionsTexts[i].setFillColor(sf::Color::White);
            m_optionsTexts[i].setScale(1.25f, 1.25f);
        } else {
            // DEFAULT STATE: Yellow color and normal scale
            m_optionsTexts[i].setFillColor(sf::Color::Yellow);
            m_optionsTexts[i].setScale(1.0f, 1.0f);
        }
        window.draw(m_optionsTexts[i]);
    }
}
