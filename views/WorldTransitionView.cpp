#include "WorldTransitionView.h"

namespace View {

    WorldTransitionView::WorldTransitionView() {}

    void WorldTransitionView::init(sf::Font& sharedFont) {
        m_font = sharedFont;

        // Configure Level Text (Top-Center)
        m_textLevel.setFont(m_font);
        m_textLevel.setCharacterSize(50);
        m_textLevel.setFillColor(sf::Color::White);

        // Configure Lives Text (Bottom-Center)
        m_textLives.setFont(m_font);
        m_textLives.setCharacterSize(50);
        m_textLives.setFillColor(sf::Color::White);

        // Load the Ninja icon to represent the player
        // Using the first frame of the idle animation
        if (!m_iconTexture.loadFromFile("resources/monochrome_ninja/idle/0.png")) {
            // Error handling handled by SFML console output
        }
        m_iconSprite.setTexture(m_iconTexture);
        m_iconSprite.setScale(4.0f, 4.0f); // Make it large for the transition screen
    }

    void WorldTransitionView::draw(sf::RenderWindow& window, const WorldTransitionModel& model) {
        // Clear window with absolute black
        window.clear(sf::Color::Black);

        // 1. Position and draw Level Text
        m_textLevel.setString(model.getLevelName());
        sf::FloatRect levelBounds = m_textLevel.getLocalBounds();
        // Center horizontally
        m_textLevel.setOrigin(levelBounds.left + levelBounds.width / 2.0f, 0);
        m_textLevel.setPosition(window.getSize().x / 2.0f, 200.0f);

        // 2. Position the Ninja Icon
        m_iconSprite.setOrigin(16.0f, 16.0f); // Center of 32x32 sprite
        m_iconSprite.setPosition(window.getSize().x / 2.0f - 60.0f, 400.0f);

        // 3. Position and draw Lives Count
        m_textLives.setString("x   " + std::to_string(model.getLives()));
        m_textLives.setPosition(window.getSize().x / 2.0f + 20.0f, 375.0f);

        window.draw(m_textLevel);
        window.draw(m_iconSprite);
        window.draw(m_textLives);
    }
}
