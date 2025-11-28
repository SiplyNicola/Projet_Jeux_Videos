#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

// On inclut les headers de tes classes
#include "../models/PlayerModel.h"
#include "../views/PlayerView.h"
#include "../controllers/PlayerController.h"

// Si tu as gardé le boss et le niveau
#include "../models/BossModel.h"
#include "../views/BossView.h"
#include "../models/LevelModel.h"
#include "../views/LevelView.h"

#include "../views/Background.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions(); // Si tu as ajouté cette fonction

    sf::RenderWindow m_window;
    sf::View m_camera;

    View::Background m_background; // Note le namespace "View::"

    // --- C'EST ICI QU'IL MANQUAIT PROBABLEMENT LA DÉCLARATION ---
    PlayerModel m_playerModel;         // <--- Indispensable pour que ça marche !
    PlayerView m_playerView;
    PlayerController m_playerController;

    // Tes autres objets (Boss, Level...)
    BossModel m_boss;
    BossView m_bossView;
    LevelModel m_level;
    LevelView m_levelView;
};

#endif
