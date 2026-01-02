#include "InputHandler.h"
#include "AttackCommand.h"
#include "DashCommand.h"
#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "JumpCommand.h"
#include "InteractCommand.h"
#include "PauseCommand.h"

InputHandler::InputHandler(PlayerModel& player, sf::FloatRect guardianZone, std::function<void()> onSucces, std::function<void()> onPause)
{
    // On lie chaque touche à une commande spécifique
    m_bindings[sf::Keyboard::Q] = std::make_unique<MoveLeftCommand>(player);  //
    m_bindings[sf::Keyboard::D] = std::make_unique<MoveRightCommand>(player); //
    m_bindings[sf::Keyboard::Z] = std::make_unique<JumpCommand>(player);
    m_bindings[sf::Keyboard::K] = std::make_unique<AttackCommand>(player);    //
    m_bindings[sf::Keyboard::J] = std::make_unique<DashCommand>(player);      //
    m_bindings[sf::Keyboard::E] = std::make_unique<InteractCommand>(player, guardianZone, onSucces);
    m_bindings[sf::Keyboard::M] = std::make_unique<PauseCommand>(onPause);
}

std::vector<ICommand*> InputHandler::handleInput()
{
    std::vector<ICommand*> activeCommands;

    // On parcourt la map pour vérifier quelles touches sont enfoncées (synchrone)
    for (auto const& [key, command] : m_bindings)
    {
        if (sf::Keyboard::isKeyPressed(key))
        {
            activeCommands.push_back(command.get());
        }
    }

    return activeCommands;
}
