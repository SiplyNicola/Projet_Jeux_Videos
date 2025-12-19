#include "PlayerView.h"
#include <iostream>

// Chargement Sécurisé Ninja
void PlayerView::loadAnimation(PlayerState state, std::string folderName, int imageCount) {
    std::vector<sf::Texture>& textureList = animationTextures[state];
    std::string basePath = "resources/monochrome_ninja/" + folderName + "/";

    for (int i = 0; i < imageCount; ++i) {
        sf::Texture tex;
        std::string filename = basePath + std::to_string(i) + ".png";

        if (tex.loadFromFile(filename)) {
            tex.setSmooth(false);
            textureList.push_back(tex);
        } else {
            std::cerr << "Info: Fichier manquant ignore : " << filename << std::endl;
        }
    }
}

// Chargement Sécurisé Epée
void PlayerView::loadSwordEffect(int imageCount) {
    std::string basePath = "resources/monochrome_ninja/sword/";
    swordTextures.clear();

    for (int i = 0; i < imageCount; ++i) {
        sf::Texture tex;
        std::string filename = basePath + std::to_string(i) + ".png";

        if (tex.loadFromFile(filename)) {
            tex.setSmooth(false);
            swordTextures.push_back(tex);
        } else {
            std::cerr << "Info: Epee manquante ignoree : " << filename << std::endl;
        }
    }
}

PlayerView::PlayerView() {
    loadAnimation(PlayerState::RUN,    "run",     10);
    loadAnimation(PlayerState::IDLE,   "idle",    10);
    loadAnimation(PlayerState::JUMP,   "jump",    5);
    loadAnimation(PlayerState::ATTACK, "attack",  5);
    loadAnimation(PlayerState::DASH,   "dashing", 5);
    loadAnimation(PlayerState::FALL,   "fall",    7);

    std::string maxFallPath = "resources/monochrome_ninja/max_fall/";
    for (int i = 0; i < 7; i++) {
        sf::Texture tex;
        if (tex.loadFromFile(maxFallPath + std::to_string(i) + ".png")) {
            tex.setSmooth(false);
            animationTextures[PlayerState::FALL].push_back(tex);
        }
    }

    loadSwordEffect(4);
    m_lastState = PlayerState::IDLE;

    float scaleInit = 1.5f;
    sprite.setScale(scaleInit, scaleInit);
    swordSprite.setScale(scaleInit, scaleInit);

    currentFrame = 0;
    timeElapsed = 0;

    if (!animationTextures[PlayerState::IDLE].empty()) {
        sprite.setTexture(animationTextures[PlayerState::IDLE][0]);
        sf::Vector2u size = animationTextures[PlayerState::IDLE][0].getSize();
        sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    }
    swordSprite.setColor(sf::Color::Transparent);
}

void PlayerView::updateAnimation(const PlayerModel& model, float deltaTime) {

    if (model.state != m_lastState) {
        currentFrame = 0;
        timeElapsed = 0;
        m_lastState = model.state;
    }

    PlayerState stateToUse = model.state;
    if (animationTextures[stateToUse].empty()) stateToUse = PlayerState::IDLE;

    const std::vector<sf::Texture>& currentAnimList = animationTextures[stateToUse];
    if (currentAnimList.empty()) return;

    switch(model.state) {
        case PlayerState::RUN:    frameDuration = 0.10f; break;
        case PlayerState::ATTACK: frameDuration = 0.06f; break;
        case PlayerState::DASH:   frameDuration = 0.05f; break;
        case PlayerState::JUMP:   frameDuration = 0.10f; break;
        case PlayerState::FALL:   frameDuration = 0.10f; break;
        default:                  frameDuration = 0.15f; break;
    }

    timeElapsed += deltaTime;
    if (timeElapsed >= frameDuration) {
        currentFrame++;
        timeElapsed = 0;

        if (currentFrame >= currentAnimList.size()) {
            if (model.state == PlayerState::FALL) {
                currentFrame = 7;
            }
            else if (model.state == PlayerState::JUMP) {
                currentFrame = currentAnimList.size() - 1;
            } else {
                currentFrame = 0;
            }
        }
    }

    sprite.setTexture(currentAnimList[currentFrame]);
    sf::Vector2u size = currentAnimList[currentFrame].getSize();
    sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);

    // MODIF 1 : model.facingDirection est remplacé par model.isFacingRight()
    float scaleVal = 2.0f;
    float dir = model.isFacingRight() ? 1.0f : -1.0f;
    sprite.setScale(scaleVal * dir, scaleVal);

    if (model.isDashing) sprite.setColor(sf::Color(0, 0, 0, 100));
    else sprite.setColor(sf::Color::White);

    float visualOffset = 20.0f;

    // MODIF 2 : model.position est remplacé par model.getPosition()
    sf::Vector2f pos = model.getPosition();
    sprite.setPosition(pos.x, pos.y - visualOffset);

    // --- B. ÉPÉE ---
    if (model.state == PlayerState::ATTACK && !swordTextures.empty()) {
        swordSprite.setColor(sf::Color::White);

        int swordFrame = currentFrame;
        if (swordFrame >= swordTextures.size()) swordFrame = swordTextures.size() - 1;

        swordSprite.setTexture(swordTextures[swordFrame]);
        sf::Vector2u swordSize = swordTextures[swordFrame].getSize();
        swordSprite.setOrigin(swordSize.x / 2.0f, swordSize.y / 2.0f);

        // MODIF 3 : Utilisation de pos (récupérée via getPosition) et dir
        swordSprite.setPosition(pos.x, pos.y - visualOffset);
        swordSprite.setScale(scaleVal * dir, scaleVal);
    } else {
        swordSprite.setColor(sf::Color::Transparent);
    }

    if (currentFrame >= currentAnimList.size()) {
        currentFrame = 0;
    }
}

void PlayerView::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(swordSprite);
}

sf::Vector2f PlayerView::getPosition() const {
    return sprite.getPosition();
}
