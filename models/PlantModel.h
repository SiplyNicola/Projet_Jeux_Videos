#ifndef PLANT_MODEL_H
#define PLANT_MODEL_H

#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>

enum PlantState { P_IDLE, P_ATTACKING };

class PlantModel : public Entity {
public:
    PlantModel(sf::Vector2f pos);
    void update(float dt) override;

    PlantState getState() const { return m_state; }
    float getTimer() const { return m_timer; }
    sf::FloatRect getBiteZone() const; // Zone dangereuse

private:
    PlantState m_state;
    float m_timer;
    const float ATTACK_INTERVAL = 3.0f; // Attaque toutes les 3 sec
};

#endif
