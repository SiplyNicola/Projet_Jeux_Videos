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
    bool m_isGrounded;

    PlayerModel();

    void move(float dirX);
    void jump();
    void attack();
    void dash();


    sf::FloatRect getHitbox() const override;
    void update(float deltaTime) override;
    void takeDamage(int amount) override;

    void addCoin() { m_coins++; }
    int getCoins() const { return m_coins; }

private:
    int m_coins; // Le compteur de pièces
};

#endif
