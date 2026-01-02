#ifndef INTERACTCOMMAND_H
#define INTERACTCOMMAND_H

#include "ICommand.h"
#include "PlayerModel.h"
#include <SFML/Graphics/Rect.hpp>
#include <functional>

class InteractCommand : public ICommand
{
    public:
        // On passe : le joueur, la zone du PNJ, et la fonction à appeler en cas de victoire
        InteractCommand(PlayerModel& player, const sf::FloatRect& guardianZone, std::function<void()> onVictory);

        void execute() override;

    private:
        PlayerModel& m_player;
        const sf::FloatRect m_zone;
        std::function<void()> m_onVictory;
};

#endif
