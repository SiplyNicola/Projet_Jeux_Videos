#ifndef WORLDTRANSITIONVIEW_H
#define WORLDTRANSITIONVIEW_H

#include <SFML/Graphics.hpp>
#include "WorldTransitionModel.h"

namespace View {
    class WorldTransitionView {
    public:
        WorldTransitionView();

        /**
         * Loads textures and font for the transition screen.
         */
        void init(sf::Font& sharedFont);

        /**
         * Renders the Mario-style black screen.
         */
        void draw(sf::RenderWindow& window, const WorldTransitionModel& model);

    private:
        sf::Font    m_font;
        sf::Text    m_textLevel;   // "WORLD 1-X" text
        sf::Text    m_textLives;   // "x 1" text
        sf::Texture m_iconTexture; // Texture for the player head icon
        sf::Sprite  m_iconSprite;  // Sprite for the player head icon
    };
}

#endif
