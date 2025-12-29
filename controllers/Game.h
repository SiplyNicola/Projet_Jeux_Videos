#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

// Inclusions des classes mod�les, vues et contr�leurs
#include "../models/PlayerModel.h"
#include "../views/PlayerView.h"
#include "../controllers/PlayerController.h"
#include "../models/BossModel.h"
#include "../views/BossView.h"
#include "../models/LevelModel.h"
#include "../views/LevelView.h"
#include "../views/Background.h"
#include "../views/HudView.h"
#include "../models/PlantModel.h"
#include "../views/PlantView.h"
#include "../models/SnakeModel.h"
#include "../views/SnakeView.h"

class Game {
public:
    Game(sf::RenderWindow& window);
    void run();

private:

    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions();
    void handleBossCollisions();
    void handleCombat(); // Nouvelle m�thode pour les d�g�ts
    void loadCaveLevel(); // <--- AJOUT : Fonction pour changer de niveau
    void handleSnakeCollisions();

    sf::RenderWindow& m_window;
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

    // <--- AJOUT : Variables pour g�rer le niveau
    int m_currentLevelId;
    sf::FloatRect m_nextLevelTrigger;
    PlantModel m_plantModel;
    PlantView m_plantView;

    SnakeModel m_snakeModel;
    SnakeView m_snakeView;
};

#endif
