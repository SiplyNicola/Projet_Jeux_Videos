#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include "ICommand.h"
#include "PlayerModel.h"
#include "../../views/SoundManager.h"

class InputHandler
{
    public:
        explicit InputHandler(PlayerModel& player, SoundManager& soundManager, sf::FloatRect guardianZone, std::function<void()> onSucces, std::function<void()> onPause);
        ~InputHandler() = default;

        // Retourne la liste de toutes les commandes dont la touche est pressée
        std::vector<ICommand*> handleInput();

    private:
        // Map associant une touche SFML à une commande unique
        std::map<sf::Keyboard::Key, std::unique_ptr<ICommand>> m_bindings;
};

#endif // INPUTHANDLER_H
