#include "Background.h"
#include <iostream>

namespace View {

    /**
     * Background Constructor
     * Initializes the background system with the current window dimensions.
     * @param windowSize The resolution of the game window (e.g., 1280x720).
     */
    Background::Background(sf::Vector2u windowSize) : m_windowSize(windowSize)
    {
        // MEMORY OPTIMIZATION: Reserve memory for layers to prevent vector reallocation.
        // This is a critical fix for the "white square" bug. Reallocation moves textures
        // in memory, which breaks the pointers held by the sprites.
        m_layers.reserve(10);
    }

    /**
     * Adds a new image layer to the background system.
     * Handles texture loading, infinite repetition, and automatic scaling.
     * @param path The file path to the background image.
     */
    void Background::addLayer(const std::string& path) {
        // Construct the Layer object directly inside the vector to maintain memory stability.
        m_layers.emplace_back();
        Layer& layer = m_layers.back();

        // LOAD TEXTURE: Attempt to load the image and log an error if the path is invalid.
        if (!layer.texture.loadFromFile(path)) {
            std::cerr << "Error: Unable to load " << path << std::endl;
            return;
        }

        // INFINITE SCROLLING: Enable texture repetition so the background can loop.
        layer.texture.setRepeated(true);
        layer.sprite.setTexture(layer.texture);

        // DYNAMIC SCALING: Calculate a zoom factor so the image height
        // matches the window height perfectly.
        float scale = (float)m_windowSize.y / layer.texture.getSize().y;
        layer.sprite.setScale(scale, scale);

        // TEXTURE RECT CALCULATION: Determine the width needed to fill the screen.
        int widthToRead = static_cast<int>(m_windowSize.x / scale);

        // Define the display area. We add 2 extra pixels to the width to prevent
        // visual artifacts or "seams" during horizontal scrolling.
        layer.sprite.setTextureRect(sf::IntRect(0, 0, widthToRead + 2, layer.texture.getSize().y));
    }

    /**
     * Updates the position of all background layers.
     * Synchronizes the background with the camera so it follows the player.
     * @param cameraX The top-left X coordinate of the current view.
     * @param cameraY The top-left Y coordinate of the current view.
     */
    void Background::update(float cameraX, float cameraY) {
        for (auto& layer : m_layers) {
            // Position each sprite exactly at the camera's coordinates.
            // This ensures the background always covers the player's screen
            // regardless of their position in the large game world.
            layer.sprite.setPosition(cameraX, cameraY);
        }
    }

    /**
     * Renders all background layers to the window.
     * Layers are drawn in the order they were added (back-to-front).
     */
    void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for (const auto& layer : m_layers) {
            target.draw(layer.sprite, states);
        }
    }
}
