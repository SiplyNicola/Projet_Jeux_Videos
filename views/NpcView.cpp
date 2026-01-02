#include "NpcView.h"
#include <iostream>

NpcView::NpcView() {
    // Constructeur vide ou initialisations de base si besoin
}

void NpcView::init() {
    // 1. Chargement de l'image (renommée guardian.png)
    if (!m_texture.loadFromFile("resources/guardian.png")) {
        // Chemin de secours au cas où le dossier resources n'est pas trouvé
        if (!m_texture.loadFromFile("guardian.png")) {
            std::cerr << "ERREUR CRITIQUE : Impossible de charger guardian.png" << std::endl;
        }
    }

    m_sprite.setTexture(m_texture);

    // 2. Découpage (Texture Rect)
    // On prend le premier carré de 32x32 pixels en haut à gauche
    // Cela correspond à la pose "Idle" (immobile) de ta planche
    m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    // 3. Mise à l'échelle
    // On double la taille pour qu'il ne soit pas tout petit
    m_sprite.setScale(2.0f, 2.0f);

    // 4. Point d'origine (Pivot)
    // On met l'origine aux pieds du personnage (milieu bas : x=16, y=32)
    // Comme ça, quand on le place en (x, y), ses pieds touchent le sol.
    m_sprite.setOrigin(16.0f, 32.0f);
}

void NpcView::draw(sf::RenderWindow& window, sf::Vector2f position) {
    m_sprite.setPosition(position);
    window.draw(m_sprite);
}
