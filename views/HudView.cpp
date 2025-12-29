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

    // --- AJOUT : CONFIGURATION DU TEXTE DE DEBUG ---

    // 1. Charger la police (Mets un fichier .ttf valide ici !)
    // Si tu n'as pas de dossier fonts, crée-le ou change le chemin.
    if (!m_font.loadFromFile("resources/fonts/PTBLDDSK.ttf")) {
        std::cerr << "ERREUR : Police introuvable (resources/fonts/PTBLDDSK.ttf)" << std::endl;
        // Pas de return ici, sinon le jeu plante si la police manque.
        // Le texte ne s'affichera juste pas.
    }

    m_coordText.setFont(m_font);
    m_coordText.setCharacterSize(24); // Taille du texte
    m_coordText.setFillColor(sf::Color::Yellow); // Jaune pour bien le voir sur fond gris
    m_coordText.setPosition(20.0f, 130.0f); // Positionné en dessous des cœurs
}

void HudView::draw(sf::RenderWindow& window, int currentHearts, sf::Vector2f playerPos) {
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

    // 2. Dessin des coordonnées (AJOUT)
    // On convertit en int pour éviter les virgules (150.43 -> 150)
    std::string coordString = "X: " + std::to_string((int)playerPos.x) +
                              "  Y: " + std::to_string((int)playerPos.y);

    m_coordText.setString(coordString);
    window.draw(m_coordText);

    // Retour à la vue du jeu
    window.setView(oldView);
}
