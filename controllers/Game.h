#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

// Existing inclusions
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

// New Pause system inclusions
#include "PauseModel.h"
#include "PauseView.h"
#include "PauseController.h"

class Game {
public:
    Game(sf::RenderWindow& m_window);
    ~Game();
    void run();

private:
    // Prevent copying due to the sf::RenderWindow reference (Coplien compliance for complex classes)
    Game(const Game& m_other) = delete;
    Game& operator=(const Game& m_other) = delete;

    void processEvents();
    void update(float m_dt);
    void render();
    void handleCollisions();
    void handleBossCollisions();
    void handleCombat();
    void loadCaveLevel();
    void handleSnakeCollisions();

    // Core SFML members
    sf::RenderWindow& m_window;
    sf::View m_camera;

    // Game logic members
    View::Background m_background;
    PlayerModel m_playerModel;
    PlayerView m_playerView;
    PlayerController m_playerController;
    HudView m_hud;
    BossModel m_boss;
    BossView m_bossView;
    LevelModel m_level;
    LevelView m_levelView;
    PlantModel m_plantModel;
    PlantView m_plantView;

    // Level and enemy management
    int m_currentLevelId;
    sf::FloatRect m_nextLevelTrigger;
    sf::Texture m_snakeTexture;
    std::vector<SnakeModel> m_snakes;
    std::vector<SnakeView> m_snakeViews;

    // --- PAUSE SYSTEM MEMBERS ---
    bool m_isPaused;
    bool m_exitToMainFlag;
    PauseModel m_pauseModel;
    PauseView m_pauseView;
    PauseController m_pauseController;
};

#endif
