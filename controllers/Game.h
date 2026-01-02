#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// Models & Views
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
#include "../models/SpiderModel.h"
#include "../views/SpiderView.h"
#include "PauseModel.h"
#include "PauseView.h"
#include "PauseController.h"
#include "../views/NpcView.h"
#include "../views/WorldTransitionView.h"
#include "../models/WorldTransitionModel.h"
#include "command/InputHandler.h"

/**
 * @brief Main Game class managing the game loop and logic.
 * Follows the Coplien Canonical Form (Destructor, Copy Constructor, Assignment Operator).
 */
class Game {
public:
    // 1. Default/Parametrized Constructor
    Game(sf::RenderWindow& window);

    // 2. Destructor (Essential for cleaning up dynamic memory in m_enemies)
    ~Game();

    void run();

private:
    // 3. Copy Constructor (Deleted)
    // Justification: The Game class owns unique resources (Window reference, unique pointers).
    // Copying a Game instance would lead to ambiguous ownership and double-free errors.
    Game(const Game& other) = delete;

    WorldTransitionModel m_transitionModel;
    View::WorldTransitionView m_transitionView;
    bool m_isTransitioning; // Flag to track if the transition screen is active
    sf::Font m_font;

    void initEntities();

    // 4. Assignment Operator (Deleted)
    Game& operator=(const Game& other) = delete;

    void processEvents();
    void update(float dt);
    void render();

    // Unified collision handling or kept separate for clarity
    void handleCollisions();
    void handleBossCollisions();
    void handleCombat();
    void loadCaveLevel();

    // Helper to separate physics logic (called inside unified update loop)
    void resolveEnemyCollision(Character* enemy);

    sf::RenderWindow& m_window;
    sf::View m_camera;

    View::Background m_background;
    PlayerModel m_playerModel;
    PlayerView m_playerView;
    std::unique_ptr<InputHandler> m_inputHandler;
    HudView m_hud;

    // Boss remains separate as it is a unique entity with specific phase logic
    BossModel m_boss;
    BossView m_bossView;

    LevelModel m_level;
    LevelView m_levelView;
   std::vector<PlantModel> m_plants;      // List of plant logic objects
    std::vector<PlantView>  m_plantViews;  // List of plant visual representations

    sf::Vector2f m_guardianPos;
    sf::FloatRect m_guardianZone;
    NpcView m_guardianView;

    int m_currentLevelId;
    sf::FloatRect m_nextLevelTrigger;

    // --- INTROSPECTION ARCHITECTURE ---
    // Instead of separate vectors, we use a single polymorphic container.
    // Justification: Allows processing all entities in a single loop (physics, combat).
    std::vector<Character*> m_enemies;

    // Views represent the "Presentation" layer and remain specific.
    // We will sync them using the index or by introspection mapping.
    std::vector<SnakeView> m_snakeViews;
    std::vector<SpiderView> m_spiderViews;

    // Pause System
    bool m_isPaused;
    bool m_exitToMainFlag;
    PauseModel m_pauseModel;
    PauseView m_pauseView;
    PauseController m_pauseController;
};

#endif
