#ifndef PLAYERMODEL_HPP
#define PLAYERMODEL_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Character.h"

//
enum class PlayerState { IDLE, RUN, JUMP, FALL, ATTACK, DASH, DEAD };

class PlayerModel : public Character{
public:
    bool m_canRevive = true; // Tracks if the player still has their second chance
    bool m_hasDealtDamage;
    PlayerState state;
    float dashCooldownTimer;
    float attackTimer;
    bool isDashing;
    float dashDurationTimer;
    bool m_isGrounded;
    bool isEaten;

    PlayerModel();

    void move(float dirX);
    bool jump();
    bool attack();
    bool dash();
    void revive();


    sf::FloatRect getHitbox() const override;
    void update(float deltaTime) override;
    void takeDamage(int amount) override;

    void addCoin() { m_coins++; }
    int getCoins() const { return m_coins; }

private:
    int m_coins = 3;
    float m_attackCooldown;

};

#endif
