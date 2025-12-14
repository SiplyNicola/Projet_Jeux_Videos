#include "PlayerView.h"
#include <iostream>

// Chargement Sécurisé Ninja
void PlayerView::loadAnimation(PlayerState state, std::string folderName, int imageCount) {
    std::vector<sf::Texture>& textureList = animationTextures[state];
    std::string basePath = "resources/monochrome_ninja/" + folderName + "/";

    for (int i = 0; i < imageCount; ++i) {
        sf::Texture tex;
        std::string filename = basePath + std::to_string(i) + ".png";

        // On n'ajoute l'image QUE si elle existe (évite le carré blanc)
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
    // 1. CHARGEMENT (Tes nombres d'images)
    loadAnimation(PlayerState::RUN,    "run",     10);
    loadAnimation(PlayerState::IDLE,   "idle",    10);
    loadAnimation(PlayerState::JUMP,   "jump",    5); // 5 images pour monter
    loadAnimation(PlayerState::ATTACK, "attack",  5);
    loadAnimation(PlayerState::DASH,   "dashing", 5);

    loadAnimation(PlayerState::FALL,   "fall",    7); // 7 images pour descendre
    std::string maxFallPath = "resources/monochrome_ninja/max_fall/";
    for (int i = 0; i < 7; i++) { // 7 images de max_fall
        sf::Texture tex;
        if (tex.loadFromFile(maxFallPath + std::to_string(i) + ".png")) {
            tex.setSmooth(false);
            // On ajoute à la liste FALL existante
            animationTextures[PlayerState::FALL].push_back(tex);
        }
    }

    loadSwordEffect(4); // Effet

    m_lastState = PlayerState::IDLE;

    // 2. RÉGLAGES
    float scaleInit = 1.5f;
    sprite.setScale(scaleInit, scaleInit);
    swordSprite.setScale(scaleInit, scaleInit);

    currentFrame = 0;
    timeElapsed = 0;

    // Init texture défaut
    if (!animationTextures[PlayerState::IDLE].empty()) {
        sprite.setTexture(animationTextures[PlayerState::IDLE][0]);
        sf::Vector2u size = animationTextures[PlayerState::IDLE][0].getSize();
        sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);
    }
    swordSprite.setColor(sf::Color::Transparent);
}

void PlayerView::updateAnimation(const PlayerModel& model, float deltaTime) {

    // --- CORRECTION DU CARRÉ BLANC ----------------------------
    if (model.state != m_lastState) {
        currentFrame = 0;   // On repart à la première image
        timeElapsed = 0;    // On remet le chrono à zéro
        m_lastState = model.state; // On mémorise le nouvel état
    }
    // -----------------------------------------------------------

    // --- A. NINJA ---
    PlayerState stateToUse = model.state;
    if (animationTextures[stateToUse].empty()) stateToUse = PlayerState::IDLE;

    const std::vector<sf::Texture>& currentAnimList = animationTextures[stateToUse];
    if (currentAnimList.empty()) return;

    // Vitesses
    switch(model.state) {
        case PlayerState::RUN:    frameDuration = 0.10f; break;
        case PlayerState::ATTACK: frameDuration = 0.06f; break;
        case PlayerState::DASH:   frameDuration = 0.05f; break;
        case PlayerState::JUMP:   frameDuration = 0.10f; break; // Rapide
        case PlayerState::FALL:   frameDuration = 0.10f; break; // Rapide
        default:                  frameDuration = 0.15f; break;
    }

    timeElapsed += deltaTime;
    if (timeElapsed >= frameDuration) {
        currentFrame++;
        timeElapsed = 0;

        // --- CORRECTION ICI ---
        if (currentFrame >= currentAnimList.size()) {
            // Si on est en train de SAUTER, TOMBER ou MOURIR, on ne boucle pas !
            // CAS SPÉCIAL : CHUTE
            if (model.state == PlayerState::FALL) {
                // On ne repart pas à 0 (début de chute),
                // mais à 7 (début de max_fall)
                currentFrame = 7;
            }
            else if (model.state == PlayerState::JUMP /*|| model.state == PlayerState::FALL*/) { //animation de chute qui boucle
                currentFrame = currentAnimList.size() - 1; // On reste figé sur la dernière image
            } else {
                currentFrame = 0; // Pour courir ou idle, on boucle
            }
        }
    }

    sprite.setTexture(currentAnimList[currentFrame]);
    sf::Vector2u size = currentAnimList[currentFrame].getSize();
    sprite.setOrigin(size.x / 2.0f, size.y / 2.0f);

    float scaleVal = 2.0f;
    sprite.setScale(scaleVal * model.facingDirection, scaleVal);

    if (model.isDashing) sprite.setColor(sf::Color(100, 255, 255, 200));
    else sprite.setColor(sf::Color::White);

    //décalage pour que le personnage marche bien sur la plateforme et non dans la plateforme
    float visualOffset = 20.0f;

    sprite.setPosition(model.position.x, model.position.y - visualOffset);

    // --- B. ÉPÉE ---
    if (model.state == PlayerState::ATTACK && !swordTextures.empty()) {
        swordSprite.setColor(sf::Color::White);

        int swordFrame = currentFrame;
        if (swordFrame >= swordTextures.size()) swordFrame = swordTextures.size() - 1;

        swordSprite.setTexture(swordTextures[swordFrame]);
        sf::Vector2u swordSize = swordTextures[swordFrame].getSize();
        swordSprite.setOrigin(swordSize.x / 2.0f, swordSize.y / 2.0f);

        swordSprite.setPosition(model.position.x, model.position.y - visualOffset);
        swordSprite.setScale(scaleVal * model.facingDirection, scaleVal);
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
