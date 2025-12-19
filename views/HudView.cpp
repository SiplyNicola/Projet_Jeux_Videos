#include "HudView.h"
#include <iostream>

HudView::HudView() {}

void HudView::init() {
    // Chargement de l'image Heart_red.png
    if (!m_texture.loadFromFile("resources/Red/Heart_red.png")) {
        std::cerr << "ERREUR : resources/Red/Heart_red.png introuvable !" << std::endl;
        return;
    }

    // --- CONFIGURATION LIGNE 3 (Cœurs Dorés) ---

    // Cœur plein (Ligne 3, Colonne 1)
    m_fullHeart.setTexture(m_texture);
    m_fullHeart.setTextureRect(sf::IntRect(0, 2048, 1024, 1024));

    // Cœur vide (Ligne 3, Colonne 3)
    m_emptyHeart.setTexture(m_texture);
    m_emptyHeart.setTextureRect(sf::IntRect(2048, 2048, 1024, 1024));

    // --- TAILLE AGRANDIE ---
    // On passe à 0.12f pour qu'ils soient bien imposants (environ 123 pixels de large)
    float heartScale = 0.12f;
    m_fullHeart.setScale(heartScale, heartScale);
    m_emptyHeart.setScale(heartScale, heartScale);
}

void HudView::draw(sf::RenderWindow& window, int currentHearts) {
    // Sauvegarde de la vue caméra
    sf::View oldView = window.getView();

    // On passe sur la vue fixe pour l'interface
    window.setView(window.getDefaultView());

    for (int i = 0; i < MAX_HEARTS; i++) {
        // --- ESPACEMENT RÉDUIT ---
        // On utilise un multiplicateur de 85.0f au lieu de 100.0f.
        // Comme les cœurs font ~123px de large, ils vont légèrement se chevaucher
        // pour un effet de style "collé" très sympa.
        sf::Vector2f pos(20.0f + (i * 85.0f), 20.0f);

        if (i < currentHearts) {
            m_fullHeart.setPosition(pos);
            window.draw(m_fullHeart);
        } else {
            m_emptyHeart.setPosition(pos);
            window.draw(m_emptyHeart);
        }
    }

    // Retour à la vue du jeu
    window.setView(oldView);
}
