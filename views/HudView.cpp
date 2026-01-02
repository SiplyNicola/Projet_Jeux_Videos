#include "HudView.h"
#include <iostream>

/**
 * HudView Constructor
 * Initializes the Heads-Up Display responsible for rendering the UI overlay.
 */
HudView::HudView() {}

/**
 * Initializes the HUD's graphical assets and text settings.
 * Loads heart textures, sets up sprite segments, and configures debug text fonts.
 */
void HudView::init() {
    // Loading the heart sprite sheet (Heart_red.png) and handling errors if the file is missing.
    if (!m_texture.loadFromFile("resources/Red/Heart_red.png")) {
        std::cerr << "ERROR: resources/Red/Heart_red.png not found!" << std::endl;
        return;
    }

    // --- ROW 3 CONFIGURATION: Golden Hearts ---
    // The sprite sheet is divided into a grid. We target Row 3 for golden heart variations.

    // Full Heart Segment: Located at Row 3, Column 1 (0, 2048 in pixel coordinates).
    m_fullHeart.setTexture(m_texture);
    m_fullHeart.setTextureRect(sf::IntRect(0, 2048, 1024, 1024));

    // Empty Heart Segment: Located at Row 3, Column 3 (2048, 2048 in pixel coordinates).
    m_emptyHeart.setTexture(m_texture);
    m_emptyHeart.setTextureRect(sf::IntRect(2048, 2048, 1024, 1024));

    // --- SCALE ADJUSTMENT: Increased Size ---
    // Scaling factor set to 0.12f to make hearts visually prominent (approximately 123 pixels wide).
    float heartScale = 0.12f;
    m_fullHeart.setScale(heartScale, heartScale);
    m_emptyHeart.setScale(heartScale, heartScale);

    // --- DEBUG COORDINATES CONFIGURATION ---

    // 1. Loading the Font: Requires a valid .ttf file to render text.
    if (!m_font.loadFromFile("resources/fonts/PTBLDDSK.ttf")) {
        std::cerr << "ERROR: Font file missing (resources/fonts/PTBLDDSK.ttf)" << std::endl;
        // Logic continues without return to prevent the game from crashing if only the font is missing.
    }

    // Configure debug text appearance and screen positioning.
    m_coordText.setFont(m_font);
    m_coordText.setCharacterSize(24); // Text size in pixels
    m_coordText.setFillColor(sf::Color::Yellow); // High-contrast color for visibility on dark backgrounds
    m_coordText.setPosition(20.0f, 130.0f); // Positioned directly below the health hearts
}

/**
 * Renders the UI elements to the window.
 * Temporarily switches the window view to a static UI layer so HUD elements follow the screen, not the camera.
 * @param window The main render target.
 * @param currentHearts The player's current health points.
 * @param playerPos The current coordinates of the player for debug display.
 */
void HudView::draw(sf::RenderWindow& window, int currentHearts, sf::Vector2f playerPos) {
    // Save the current world/camera view to restore it after rendering the HUD.
    sf::View oldView = window.getView();

    // Switch to the default static view (Standard 1:1 window coordinates) for the interface.
    window.setView(window.getDefaultView());

    // Loop through the maximum allowed hearts to render the health bar.
    for (int i = 0; i < MAX_HEARTS; i++) {
        // --- OVERLAPPING SPACING LOGIC ---
        // We use a horizontal multiplier of 85.0f.
        // Since hearts are ~123px wide, they will slightly overlap, creating a "chained" visual effect.
        sf::Vector2f pos(20.0f + (i * 85.0f), 20.0f);

        if (i < currentHearts) {
            // Draw a full golden heart if the player still has this health point.
            m_fullHeart.setPosition(pos);
            window.draw(m_fullHeart);
        } else {
            // Draw an empty heart if the health point has been lost.
            m_emptyHeart.setPosition(pos);
            window.draw(m_emptyHeart);
        }
    }

    // --- COORDINATE DISPLAY ---
    // Convert float coordinates to integers to remove decimal places for a cleaner UI.
    std::string coordString = "X: " + std::to_string((int)playerPos.x) +
                              "  Y: " + std::to_string((int)playerPos.y);

    m_coordText.setString(coordString);
    window.draw(m_coordText);

    // Restore the original game/camera view so the world continues to render correctly.
    window.setView(oldView);
}
