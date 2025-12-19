#include "PlantModel.h"

PlantModel::PlantModel(sf::Vector2f pos) {
    m_position = pos;
    m_state = P_IDLE;
    m_timer = 0;
}

void PlantModel::update(float dt) {
    m_timer += dt;

    if (m_state == P_IDLE) {
        if (m_timer >= 3.0f) { // Temps d'attente
            m_state = P_ATTACKING;
            m_timer = 0.0f;
        }
    }
    else if (m_state == P_ATTACKING) {
        // On laisse l'attaque durer un tout petit peu plus (1.1s)
        // pour que la vue puisse afficher la frame 7 sans être coupée
        if (m_timer >= 1.0f) {
            m_state = P_IDLE;
            m_timer = 0.0f;
        }
    }
}

sf::FloatRect PlantModel::getBiteZone() const {
    // On définit la zone de morsure devant la plante
    return sf::FloatRect(m_position.x - 40, m_position.y - 40, 80, 80);
}
