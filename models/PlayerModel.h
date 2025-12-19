#ifndef PLAYERMODEL_HPP
#define PLAYERMODEL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Character.h"

//
enum class PlayerState { IDLE, RUN, JUMP, FALL, ATTACK, DASH, DEAD };

class PlayerModel : public Character{
public:
    bool m_hasDealtDamage; // Pour ne frapper qu'une fois par coup
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

    sf::FloatRect getHitbox() const override;
    void update(float deltaTime) override;
    void takeDamage(int amount) override;


};

#endif
