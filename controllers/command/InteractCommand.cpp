#include "InteractCommand.h"
#include "iostream"
#include "SFML/Graphics/Rect.hpp"
#include "functional"

InteractCommand::InteractCommand(PlayerModel& player, const sf::FloatRect& guardianZone, std::function<void()> onVictory)
    : m_player(player), m_zone(guardianZone), m_onVictory(onVictory) {}

void InteractCommand::execute()
{
    // On vérifie la collision entre la hitbox du joueur et la zone du gardien
    if (m_player.getHitbox().intersects(m_zone))
    {
        if (m_player.getCoins() >= 3)
        {
            std::cout << "Guardian: You have the 3 coins. Cross the bridge!" << std::endl;
            // On appelle la fonction de changement de niveau (loadCaveLevel)
            m_onVictory();
        }
        else
        {
            std::cout << "Guardian: You need 3 coins!" << std::endl;
            // On repousse le joueur
            m_player.setPosition(m_player.getPosition().x - 20, m_player.getPosition().y);
        }
    }
}
