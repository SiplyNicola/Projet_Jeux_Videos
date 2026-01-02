#ifndef ITEMS_H
#define ITEMS_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include "../../models/PlayerModel.h"

class Items {
    public:
        Items(sf::Vector2f position, std::string name);
        virtual ~Items() = default;

        virtual bool applyEffect(PlayerModel& player) = 0;

        sf::FloatRect getHitbox() const;
        sf::Vector2f getPosition() const { return m_position; }

    protected:
        sf::Vector2f m_position;
        std::string m_name;
};

#endif
