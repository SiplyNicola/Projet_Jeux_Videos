#ifndef HEALTHPOTION_H
#define HEALTHPOTION_H

#include "Items.h"

class HealthPotion : public Items {
public:
    HealthPotion(sf::Vector2f position);
    bool applyEffect(PlayerModel& player) override;
};

#endif
