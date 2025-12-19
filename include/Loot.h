#ifndef LOOT_H
#define LOOT_H

#include "Entity.h"
#include <SFML/Graphics/Rect.hpp>
#include <string>

class Loot : public Entity {
public:
    // Le constructeur prend le type (ex: "HEART") et la position de départ
    Loot(std::string type, sf::Vector2f pos);

    // Obligatoire car update est virtuel pur dans Entity
    void update(float dt) override;

    // Pour détecter si le joueur marche dessus
    sf::FloatRect getHitbox() const;

    std::string getType() const { return m_type; }
    bool isCollected() const { return m_collected; }
    void collect() { m_collected = true; }

private:
    std::string m_type;
    bool m_collected;
    float m_floatingTimer; // Pour l'animation visuelle
    float m_startY;        // Pour garder la position d'origine pendant l'animation
};

#endif
