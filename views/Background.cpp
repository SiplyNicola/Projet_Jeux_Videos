#include "Background.h"
#include <iostream>

namespace View {

    Background::Background(sf::Vector2u windowSize) : m_windowSize(windowSize)
    {
        // Reserve memory to prevent vector reallocation (avoids "white square" bug).
        m_layers.reserve(10);
    }

    void Background::addLayer(const std::string& path) {
        // Construct the layer directly in the vector.
        m_layers.emplace_back();
        Layer& layer = m_layers.back();

        // Load texture and check for errors.
        if (!layer.texture.loadFromFile(path)) {
            std::cerr << "Error: Unable to load " << path << std::endl;
            return;
        }

        // Enable repetition for infinite scrolling.
        layer.texture.setRepeated(true);
        layer.sprite.setTexture(layer.texture);


        // Calculate zoom so the image height matches the window height.
        float scale = (float)m_windowSize.y / layer.texture.getSize().y;
        layer.sprite.setScale(scale, scale);

        // Calculate the width of the texture to display to fill the screen width.
        int widthToRead = static_cast<int>(m_windowSize.x / scale);

        // Set the texture rect (area to display). +2 pixels to avoid visual artifacts.
        layer.sprite.setTextureRect(sf::IntRect(0, 0, widthToRead + 2, layer.texture.getSize().y));
    }

    void Background::update(float cameraX, float cameraY) {
    for (auto& layer : m_layers) {
        // On place le sprite à la position exacte de la caméra (X et Y)
        // Comme ça, le coin haut-gauche de l'image est toujours collé au coin haut-gauche de l'écran
        layer.sprite.setPosition(cameraX, cameraY);
    }
}

    void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for (const auto& layer : m_layers) {
            target.draw(layer.sprite, states);
        }
    }
}
