#ifndef PLAYERMODEL_HPP
#define PLAYERMODEL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

//
enum class PlayerState { IDLE, RUN, JUMP, FALL, ATTACK, DASH };

class PlayerModel {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    int facingDirection;
    PlayerState state;

    float dashCooldownTimer;
    float attackTimer;
    bool isDashing;
    float dashDurationTimer;

    PlayerModel();
    void move(float dirX);
    void jump();
    void attack();
    void dash();
    void update(float deltaTime);

    sf::FloatRect getHitbox() const;
};

#endif
