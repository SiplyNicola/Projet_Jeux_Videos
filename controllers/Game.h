#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

// Inclusions des classes modèles, vues et contrôleurs
#include "../models/PlayerModel.h"
#include "../views/PlayerView.h"
#include "../controllers/PlayerController.h"
#include "../models/BossModel.h"
#include "../views/BossView.h"
#include "../models/LevelModel.h"
#include "../views/LevelView.h"
#include "../views/Background.h"
#include "../views/HudView.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions();
    void handleBossCollisions();
    void handleCombat(); // Nouvelle méthode pour les dégâts
    void loadCaveLevel(); // <--- AJOUT : Fonction pour changer de niveau

    sf::RenderWindow m_window;
    sf::View m_camera;

    View::Background m_background;

    PlayerModel m_playerModel;
    PlayerView m_playerView;
    PlayerController m_playerController;

    HudView m_hud;

    BossModel m_boss;
    BossView m_bossView;
    LevelModel m_level;
    LevelView m_levelView;

    // <--- AJOUT : Variables pour gérer le niveau
    int m_currentLevelId;
    sf::FloatRect m_nextLevelTrigger;
};

#endif
