#ifndef PLAYERVIEW_HPP
#define PLAYERVIEW_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "PlayerModel.h"

class PlayerView {
private:
    sf::Sprite sprite;      // Ninja
    sf::Sprite swordSprite; // Effet
    PlayerState m_lastState;

    std::map<PlayerState, std::vector<sf::Texture>> animationTextures;
    std::vector<sf::Texture> swordTextures;

    int currentFrame;
    float timeElapsed;
    float frameDuration;

    void loadAnimation(PlayerState state, std::string folderName, int imageCount);
    void loadSwordEffect(int imageCount);

public:
    PlayerView();
    void updateAnimation(const PlayerModel& model, float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
};

#endif
