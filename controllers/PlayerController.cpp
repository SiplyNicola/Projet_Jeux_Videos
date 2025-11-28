#include "PlayerController.h"
#include <SFML/Window/Keyboard.hpp>

void PlayerController::handleInput(PlayerModel& model) {
    if (model.state == PlayerState::ATTACK || model.isDashing) return;

    float moveDir = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveDir = 1.0f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) moveDir = -1.0f;

    model.move(moveDir);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) model.jump();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) model.attack();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) model.dash();
}
