#ifndef BOSS_MODEL_H
#define BOSS_MODEL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

enum BossState { IDLE, WALKING, ATTACKING, HURT, DEAD };

class BossModel {
public:
    BossModel();
    void update(float dt, sf::Vector2f playerPos);
    void takeDamage(int amount);

    sf::Vector2f getPosition() const;
    BossState getState() const;
    bool isFacingRight() const;
    bool isDead() const;
    sf::FloatRect getHitbox() const;

    void setPosition(float x, float y); // Pour le téléporter sur le sol
    void setVelocity(sf::Vector2f v);   // Pour arrêter sa chute
    sf::Vector2f getVelocity() const;   // Pour savoir s'il tombe

private:
    sf::Vector2f m_position;
    BossState m_state;
    int m_hp;
    float m_speed;
    bool m_facingRight;
    float m_stateTimer;
    float m_attackCooldown;
    sf::Vector2f m_velocity;
};
#endif
