#include "NpcView.h"
#include <iostream>

/**
 * NpcView Constructor
 * Initializes the visual representation class for NPCs (Non-Player Characters).
 */
NpcView::NpcView() {
    // Default constructor or base initializations if required.
}

/**
 * Initializes the NPC's graphical assets.
 * Handles texture loading with fallback paths, sprite clipping, scaling, and pivot alignment.
 */
void NpcView::init() {
    // 1. Texture Loading (guardian.png)
    // Attempts to load the asset from the standard resources directory.
    if (!m_texture.loadFromFile("resources/guardian.png")) {
        // Backup path attempt in case the executable is running from a different relative root.
        if (!m_texture.loadFromFile("guardian.png")) {
            std::cerr << "CRITICAL ERROR: Unable to load guardian.png" << std::endl;
        }
    }

    m_sprite.setTexture(m_texture);

    m_animTimer = 0.0f;
    m_currentFrame = 0;

    // 2. Texture Rect (Clipping/Framing)
    // Extracts the first 32x32 pixel square from the top-left of the sprite sheet.
    // This typically represents the "Idle" (standing still) pose for the character.
    m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    // 3. Scaling
    // Doubles the visual size of the sprite (200%) to ensure visibility within the game world.
    m_sprite.setScale(-2.0f, 2.0f);

    // 4. Origin Point (Pivot)
    // Sets the local origin to the character's feet (Bottom-Center: x=16, y=32).
    // This ensures that when positioned at a specific coordinate, the character stands ON the ground rather than being centered.
    m_sprite.setOrigin(16.0f, 32.0f);
}

/**
 * Renders the NPC sprite to the active window.
 * @param window The SFML RenderWindow target.
 * @param position The world coordinates (Vector2f) where the NPC should be drawn.
 */
void NpcView::draw(sf::RenderWindow& window, sf::Vector2f position) {
    m_sprite.setPosition(position);
    window.draw(m_sprite);
}

void NpcView::updateAnimation(float dt) {
    m_animTimer += dt;

    // Change d'image toutes les 0.5 secondes (tu peux changer ce chiffre pour accélérer/ralentir)
    if (m_animTimer >= 0.3f) {
        m_animTimer = 0.0f; // Reset le timer

        // On passe à la frame suivante (0 -> 1 -> 0 -> 1...)
        m_currentFrame = (m_currentFrame + 1) % 2;

        // Calcul du rectangle de texture
        // Frame 0 : x = 0
        // Frame 1 : x = 32 (car ton sprite fait 32 de large)
        int left = m_currentFrame * 32;

        m_sprite.setTextureRect(sf::IntRect(left, 0, 32, 32));
    }
}
