#include "PlayerController.h"
#include <SFML/Window/Keyboard.hpp>

/**
 * Handles user input to control the player's actions.
 * This method maps physical key presses to logical methods in the PlayerModel.
 * * @param model A reference to the PlayerModel to be updated based on input.
 */
void PlayerController::handleInput(PlayerModel& model, SoundManager& soundManager) {
    // INPUT GUARD: Disable control if the player is currently performing an animation
    // that locks movement, such as attacking or dashing.
    if (model.state == PlayerState::ATTACK || model.isDashing) return;

    float moveDir = 0.0f;

    // HORIZONTAL MOVEMENT
    // Uses 'D' for Right and 'Q' for Left (Standard for AZERTY keyboard layouts)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir = 1.0f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) moveDir = -1.0f;

    // Apply the calculated direction to the model's movement logic
    model.move(moveDir);

    // JUMPING: Triggered by the 'Z' key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        // On tente le saut. Si le modèle renvoie TRUE, on joue le son.
        if (model.jump()) {
            soundManager.playJump();
        }
    }

    // COMBAT: Triggered by the 'K' key to start an attack state
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) model.attack();

    // ABILITY: Triggered by the 'J' key to perform a dash maneuver
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)){
        soundManager.playDash();
        model.dash();
    }
}
