#ifndef SUSHIPLATTER_H
#define SUSHIPLATTER_H

#include <Items.h>

class SushiPlatter : public Items {
public:
    SushiPlatter(sf::Vector2f position);
    bool applyEffect(PlayerModel& player) override;
};

#endif
