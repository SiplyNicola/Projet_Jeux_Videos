#include "PlayerView.h"
#include <iostream>

/**
 * Safe Loading for Ninja Animations
 * Populates the texture vector for a specific state by loading numbered PNG files from a folder.
 * @param state The PlayerState associated with this animation.
 * @param folderName The sub-folder name containing the frames.
 * @param imageCount The total number of frames to attempt to load.
 */
void PlayerView::loadAnimation(PlayerState state, std::string folderName, int imageCount) {
    std::vector<sf::Texture>& textureList = animationTextures[state];
    std::string basePath = "resources/monochrome_ninja/" + folderName + "/";

    for (int i = 0; i < imageCount; ++i) {
        sf::Texture tex;
        // Construct filename based on index (e.g., "0.png", "1.png", etc.)
        std::string filename = basePath + std::to_string(i) + ".png";

        if (tex.loadFromFile(filename)) {
            tex.setSmooth(false); // Disable smoothing to preserve pixel art crispness
            textureList.push_back(tex);
        } else {
            // Log missing files but continue the loop to load whatever is available
            std::cerr << "Info: Missing file ignored: " << filename << std::endl;
        }
    }
}

/**
 * Safe Loading for Sword Attack Effects
 * Loads the overlay textures used during the attack animation.
 * @param imageCount The number of frames in the sword slash sequence.
 */
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
            std::cerr << "Info: Missing sword effect ignored: " << filename << std::endl;
        }
    }
}

/**
 * PlayerView Constructor
 * Initializes all animation sets, scales, and origin points for the player character.
 */
PlayerView::PlayerView() {
    // Load standard animations from the resource folders
    loadAnimation(PlayerState::RUN,    "run",      10);
    loadAnimation(PlayerState::IDLE,   "idle",     10);
    loadAnimation(PlayerState::JUMP,   "jump",      5);
    loadAnimation(PlayerState::ATTACK, "attack",    5);
    loadAnimation(PlayerState::DASH,   "dashing",   5);
    loadAnimation(PlayerState::FALL,   "fall",      7);

    // Specific loading for "max_fall" which is added to the FALL state list
    std::string maxFallPath = "resources/monochrome_ninja/max_fall/";
    for (int i = 0; i < 7; i++) {
        sf::Texture tex;
        if (tex.loadFromFile(maxFallPath + std::to_string(i) + ".png")) {
            tex.setSmooth(false);
            animationTextures[PlayerState::FALL].push_back(tex);
        }
    }

    // Load graphical effects for combat
    loadSwordEffect(4);
    m_lastState = PlayerState::IDLE;

    // Initial scale configuration
    float scaleInit = 1.5f;
    sprite.setScale(scaleInit, scaleInit);
    swordSprite.setScale(scaleInit, scaleInit);

    // Animation internal counters
    currentFrame = 0;
    timeElapsed = 0;

    // Set initial default texture and center the origin point
    if (!animationTextures[PlayerState::IDLE].empty()) {
        sprite.setTexture(animationTextures[PlayerState::IDLE][0]);
        sf::Vector2u size = animationTextures[PlayerState::IDLE][0].getSize();
        sprite.setOrigin(size.x / 2.0f, size.y / 2.0f); // Set pivot to the center of the frame
    }

    // Sword effect starts as invisible
    swordSprite.setColor(sf::Color::Transparent);
}

/**
 * Updates the visual state of the player based on the logic model.
 * Synchronizes positions, handles horizontal flipping, and progresses animation frames.
 * @param model Reference to the PlayerModel containing physics and state data.
 * @param deltaTime Time elapsed since the last frame.
 */
void PlayerView::updateAnimation(const PlayerModel& model, float deltaTime) {

    // If the logical state changed, reset animation progress to the first frame
    if (model.state != m_lastState) {
        currentFrame = 0;
        timeElapsed = 0;
        m_lastState = model.state;
    }

    // Determine which texture set to use (fallback to IDLE if requested state is empty)
    PlayerState stateToUse = model.state;
    if (animationTextures[stateToUse].empty()) stateToUse = PlayerState::IDLE;

    const std::vector<sf::Texture>& currentAnimList = animationTextures[stateToUse];
    if (currentAnimList.empty()) return;

    // Configure animation speed based on the current state (lower values = faster animation)
    switch(model.state) {
        case PlayerState::RUN:    frameDuration = 0.10f; break;
        case PlayerState::ATTACK: frameDuration = 0.06f; break;
        case PlayerState::DASH:   frameDuration = 0.05f; break;
        case PlayerState::JUMP:   frameDuration = 0.10f; break;
        case PlayerState::FALL:   frameDuration = 0.10f; break;
        default:                  frameDuration = 0.15f; break;
    }

    // Progress frame counter based on elapsed time
    timeElapsed += deltaTime;
    if (timeElapsed >= frameDuration) {
        currentFrame++;
        timeElapsed = 0;

        // Handle animation looping or clamping at the end of the sequence
        if (currentFrame >= currentAnimList.size()) {
            if (model.state == PlayerState::FALL) {
                // For falling, we lock on the "max_fall" frames (frame index 7)
                currentFrame = 7;
            }
            else if (model.state == PlayerState::JUMP) {
                // For jumping, we stay on the last ascending frame
                currentFrame = currentAnimList.size() - 1;
            } else {
                // For other states (Run, Idle), we loop back to the start
                currentFrame = 0;
            }
        }
    }

    // Apply the current frame texture to the sprite
    sprite.setTexture(currentAnimList[currentFrame]);
    sf::Vector2u size = currentAnimList[currentFrame].getSize();
    sprite.setOrigin(size.x / 2.0f, size.y / 2.0f); // Maintain centered origin

    // --- ORIENTATION & SCALING ---
    // Handle horizontal flipping based on the model's facing direction
    float scaleVal = 2.0f;
    float dir = model.isFacingRight() ? 1.0f : -1.0f; // 1.0 = Right, -1.0 = Left
    sprite.setScale(scaleVal * dir, scaleVal);

    // Apply visual feedback for the dash ability (transparency)
    if (model.isDashing) sprite.setColor(sf::Color(0, 0, 0, 100)); // Dark semi-transparent
    else sprite.setColor(sf::Color::White);

    // Positioning with a vertical offset to ensure the character stands correctly on tiles
    float visualOffset = 20.0f;
    sf::Vector2f pos = model.getPosition();
    sprite.setPosition(pos.x, pos.y - visualOffset);

    // --- B. SWORD EFFECT ---
    // If the player is in an attack state, render the sword slash overlay
    if (model.state == PlayerState::ATTACK && !swordTextures.empty()) {
        swordSprite.setColor(sf::Color::White); // Make visible

        // Sync sword frame with the character's attack frame
        int swordFrame = currentFrame;
        if (swordFrame >= swordTextures.size()) swordFrame = swordTextures.size() - 1;

        swordSprite.setTexture(swordTextures[swordFrame]);
        sf::Vector2u swordSize = swordTextures[swordFrame].getSize();
        swordSprite.setOrigin(swordSize.x / 2.0f, swordSize.y / 2.0f);

        // Position the effect relative to the player's world position and direction
        swordSprite.setPosition(pos.x, pos.y - visualOffset);
        swordSprite.setScale(scaleVal * dir, scaleVal);
    } else {
        // Hide the effect if not attacking
        swordSprite.setColor(sf::Color::Transparent);
    }

    // Final safety loop check
    if (currentFrame >= currentAnimList.size()) {
        currentFrame = 0;
    }
}

/**
 * Renders the player sprite and sword effects to the target window.
 * @param window The SFML RenderWindow target.
 */
void PlayerView::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(swordSprite);
}

/**
 * Accessor for the current visual position of the player.
 * @return sf::Vector2f containing X and Y coordinates.
 */
sf::Vector2f PlayerView::getPosition() const {
    return sprite.getPosition();
}
