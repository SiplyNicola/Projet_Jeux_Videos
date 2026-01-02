#include "SushiPlatter.h"

SushiPlatter::SushiPlatter(sf::Vector2f position) : Items(position, "Sushi Platter") {}

bool SushiPlatter::applyEffect(PlayerModel& player) {
    player.setHP(5);
    return true;
}
