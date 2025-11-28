#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace View {

    // Inheriting from sf::Drawable allows us to use "window.draw(background)"
    // respecting polymorphism.
    class Background : public sf::Drawable {
    private:
        // Internal structure to group a Texture (heavy image) and its Sprite (display).
        struct Layer {
            sf::Texture texture;
            sf::Sprite sprite;
        };

        // We use a vector to add as many layers as needed dynamically.
        std::vector<Layer> m_layers;

        // Store window size for scaling calculations.
        sf::Vector2u m_windowSize;

        // Override of the draw method to define how to render our object.
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    public:
        // Constructor needs window size for zoom calculations.
        Background(sf::Vector2u windowSize);

        // Adds a background layer from a file path.
        void addLayer(const std::string& path);

        // Updates the position of the background based on the camera position.
        void update(float cameraX, float cameraY);
    };
}
