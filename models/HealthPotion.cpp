#include "HealthPotion.h"

HealthPotion::HealthPotion(sf::Vector2f position) : Items(position, "Health Potion") {}

bool HealthPotion::applyEffect(PlayerModel& player) {
    player.setHP(player.getHP() + 1);
    return true;
}
