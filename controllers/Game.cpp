#include "Game.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

/**
 * Game Constructor
 * Initializes the window, world state, levels, and all entity lists.
 */
/**
 * Modified Constructor
 */
Game::Game(sf::RenderWindow& window)
    : m_window(window),
      m_background(sf::Vector2u(1280, 720)),
      m_isPaused(false),
      m_exitToMainFlag(false),
      m_isTransitioning(false),
      m_deathTimer(0.0f)
{
    srand(static_cast<unsigned>(time(NULL)));
    // 1. LOAD THE FONT FIRST
    if (!m_font.loadFromFile("resources/fonts/PressStart2P-regular.ttf")) {
        std::cerr << "ERROR: Could not load m_font for the Game class!" << std::endl;
    }

    m_camera.setSize(1280, 720);
    m_currentLevelId = 1;

    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel(1);
    m_levelView.build(m_level);
    m_hud.init();

    m_guardianView.init();
    m_guardianPos = sf::Vector2f(6634.0f, 432.0f);
    m_guardianZone = sf::FloatRect(m_guardianPos.x - 50.0f, m_guardianPos.y - 50.0f, 100.0f, 100.0f);
    m_pauseView.init(m_window);
    m_soundManager.init();
    m_soundManager.playLevel1Music();

    m_inputHandler = std::make_unique<InputHandler>(
        m_playerModel,
        m_soundManager,
        m_guardianZone,
        [this]() { this->loadCaveLevel(); },
        [this]() {
            this->m_isPaused = !this->m_isPaused;
            this->m_pauseModel.reset();
        }
    );

    //Items views
    m_healthPotionView.init();

    // Initialize the transition view (assuming m_font is already loaded)
    m_transitionView.init(m_font);

    // CALL THE NEW INITIALIZATION METHOD
    initEntities();

    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png");

    m_nextLevelTrigger = sf::FloatRect(3000.0f, 0.0f, 100.0f, 1000.0f);
}

/**
 * Game Destructor
 * Cleans up heap-allocated memory to prevent memory leaks.
 */
Game::~Game() {
    // Delete all enemy characters stored as pointers
    for (Character* enemy : m_enemies) {
        delete enemy;
    }
    m_enemies.clear();
    m_items.clear();
}

/**
 * Modified run loop
 * FIXED: render() is now called outside the pause/transition checks
 * so the menu can actually be drawn.
 */
void Game::run() {
    sf::Clock m_clock;

    while (m_window.isOpen() && !m_exitToMainFlag) {
        float m_dt = m_clock.restart().asSeconds();
        if (m_dt > 0.1f) m_dt = 0.1f;

        processEvents();

        // 1. UPDATE LOGIC
        if (!m_isPaused) {
            if (m_isTransitioning) {
                // Logic for the Mario-style black screen timer
                m_transitionModel.update(m_dt);

                if (m_transitionModel.isFinished()) {
                    m_isTransitioning = false;
                    m_playerModel.revive();
                    m_playerModel.setPosition(100.0f, 2520.0f);
                    m_camera.setCenter(640.0f, 2520.0f);
                    initEntities();
                    m_deathTimer = 0.0f;
                    m_clock.restart();
                }
            }
            else {
                // Standard gameplay physics and AI
                update(m_dt);

                // Check for death to trigger transition
                if (m_playerModel.isDead()) {
                        m_deathTimer += m_dt;
                        if (m_deathTimer >= 2.0f) {
                            if (m_playerModel.m_canRevive) {
                                m_isTransitioning = true;
                                m_transitionModel.reset("WORLD 1-" + std::to_string(m_currentLevelId), 1);
                            } else {
                                return; // Game Over
                            }
                        }
                }
                else {
                    // Si le joueur est vivant, le timer reste à 0
                    m_deathTimer = 0.0f;
                }
            }
        }

        // 2. RENDER LOGIC (Always called)
        render();
    }
}

/**
 * (New Method) initEntities
 * Handles the creation and cleanup of all level entities (Enemies and Plants).
 * This is used for initial setup and during player respawn.
 */
void Game::initEntities() {
    // A. CLEANUP: Delete existing enemy pointers to prevent memory leaks
    for (Character* enemy : m_enemies) {
        delete enemy;
    }
    m_enemies.clear();
    m_snakeViews.clear();
    m_spiderViews.clear();
    m_items.clear();

    // Clear plants
    m_plants.clear();
    m_plantViews.clear();

    // B. RE-CREATE PLANTS
    std::vector<sf::Vector2f> plantPositions = {
        {1669.0f, 1449.0f}, {1500.0f, 2520.0f}, {4561.0f, 366.0f}, {5829.0f, 1776.0f}
    };
    m_plantViews.reserve(plantPositions.size());
    m_plants.reserve(plantPositions.size());
    for (const auto& pos : plantPositions) {
        m_plants.push_back(PlantModel(pos));
        m_plantViews.emplace_back();
        m_plantViews.back().init();
    }

    // C. RE-CREATE SNAKES
    std::vector<sf::Vector2f> snakePositions = {
        {400.0f, 2520.0f}, {1258.0f, 2663.0f}, {1908.0f, 2520.0f}, {2961.0f, 2264.0f},
        {3780.0f, 2520.0f}, {3780.0f, 2520.0f},{7060.0f, 2230.0f},
        {7720.0f, 1780.0f},{8500.0f, 1944.0f},{6212.0f, 1871.0f},
        {5453.0f, 1655.0f},{5002.0f, 1439.0f},{2066.0f, 719.0f},
        {6680.0f, 2520.0f},{5722.0f, 2520.0f},{5054.0f, 2664.0f},
        {1153.0f, 1124.0f},{1202.0f, 1511.0f},{1357.0f, 719.0f},{4116.0f, 1008.0f}
    };
    for (const auto& pos : snakePositions) {
        m_enemies.push_back(new SnakeModel(pos.x, pos.y));
        SnakeView sView; sView.init();
        m_snakeViews.push_back(sView);
    }

    // D. RE-CREATE SPIDERS
    std::vector<sf::Vector2f> spiderPositions = {
        {1908.0f, 2400.0f},{3924.0f, 2400.0f},{4141.0f, 2400.0f},
        {5808.0f, 2400.0f},{6584.0f, 2400.0f}, {4215.0f, 1500.0f},
        {1040.0f, 1120.0f},{1117.0f, 1400.0f},{2129.0f, 1415.0f}
    };
    for (const auto& pos : spiderPositions) {
        m_enemies.push_back(new SpiderModel(pos.x, pos.y));
        SpiderView spView; spView.init();
        m_spiderViews.push_back(spView);
    }
}

/**
 * Event Processing
 * Handles window events, keyboard inputs, and menu navigation.
 */
void Game::processEvents() {
    sf::Event m_event;
    while (m_window.pollEvent(m_event)) {
        if (m_event.type == sf::Event::Closed) m_window.close();

        // Delegate input to Pause Controller if the menu is active
        if (m_isPaused) {
            m_pauseController.handleInput(m_window, m_event, m_pauseModel, m_pauseView);

            // Check for menu-specific state changes
            if (m_pauseModel.shouldResume()) {
                m_isPaused = false;
                m_pauseModel.reset();
            }
            if (m_pauseModel.shouldExitToMain()) {
                m_exitToMainFlag = true;
            }
        }
    }

    // Player movement controls: only active when alive and unpaused
    if (!m_playerModel.isDead() && !m_isPaused) {
        m_playerModel.move(0.0f);
        std::vector<ICommand*> commands = m_inputHandler->handleInput();

        for (ICommand* cmd : commands)
        {
            if (cmd) cmd->execute();
        }
    }
}

/**
 * Game World Update Logic
 * Includes out-of-bounds detection to handle falling off the map.
 */
void Game::update(float m_dt) {
    if (m_dt > 0.05f) m_dt = 0.05f;

    if (m_currentLevelId == 1 && m_playerModel.getHitbox().intersects(m_nextLevelTrigger)) {
        loadCaveLevel();
        return;
    }

    m_playerModel.update(m_dt);

    if (m_currentLevelId == 2) {
        m_boss.updateBoss(m_dt, m_playerModel.getPosition());
    }

    // --- FALL OUT OF BOUNDS DETECTION ---
    // Calculate the total height of the map based on tile size (72px)
    float m_mapH = m_level.getMapData().size() * 72.0f;
    sf::Vector2f m_pPos = m_playerModel.getPosition();

    // If player falls 100 pixels below the map floor, trigger death
    if (m_pPos.y > m_mapH + 100.0f) {
        // Deal massive damage to force the isDead() state
        m_playerModel.takeDamage(999);
    }

    // Stop processing if player is dead (avoids further physics updates)
    if (m_playerModel.isDead()) return;

    // --- ENEMY & ENTITY UPDATES ---
    int snakeIndex = 0;
    int spiderIndex = 0;
    for (Character* enemy : m_enemies) {
        if (enemy->getType() == EntityType::SNAKE) {
            SnakeModel* snake = static_cast<SnakeModel*>(enemy);
            if (!snake->isDead()) {
                snake->update(m_dt, m_playerModel.getPosition());
                if (snakeIndex < (int)m_snakeViews.size())
                    m_snakeViews[snakeIndex].update(m_dt, *snake);
            }
            snakeIndex++;
        }
        else if (enemy->getType() == EntityType::SPIDER) {
            SpiderModel* spider = static_cast<SpiderModel*>(enemy);
            if (!spider->isDead()) {
                spider->update(m_dt, m_playerModel.getPosition());
                if (spiderIndex < (int)m_spiderViews.size())
                    m_spiderViews[spiderIndex].update(m_dt, *spider);
            }
            spiderIndex++;
        }
    }

    handleCollisions();
    if (m_currentLevelId == 2) handleBossCollisions();
    handleCombat();

    // Update items
    for (auto it = m_items.begin(); it != m_items.end(); ) {
        if (m_playerModel.getHitbox().intersects((*it)->getHitbox())) {
            (*it)->applyEffect(m_playerModel);
            it = m_items.erase(it);
        } else {
            ++it;
        }
    }

    // Update Plants
    for (size_t i = 0; i < m_plants.size(); i++) {
        m_plants[i].update(m_dt);
        if (i < m_plantViews.size()) m_plantViews[i].update(m_dt, m_plants[i]);
    }

    // Camera & Background
    float m_mapW = m_level.getMapData()[0].size() * 72.0f;
    float m_cX = std::max(640.0f, std::min(m_pPos.x, m_mapW - 640.0f));
    float m_cY = std::max(360.0f, std::min(m_pPos.y, m_mapH - 360.0f));
    m_camera.setCenter(m_cX, m_cY);

    m_playerView.updateAnimation(m_playerModel, m_dt);
    if (m_currentLevelId == 2) m_bossView.update(m_dt, m_boss);
    m_background.update(m_camera.getCenter().x - 640.0f, m_camera.getCenter().y - 360.0f);
}

/**
 * Combat Handling
 * Manages attack hitzones, damage dealing, and specific hit reactions.
 */
void Game::handleCombat() {
    if (m_currentLevelId == 2) {
        // A. PLAYER ATTACKS THE BOSS
        if (m_playerModel.state == PlayerState::ATTACK && !m_playerModel.m_hasDealtDamage) {
            if (m_playerModel.attackTimer > 0.1f) {
                sf::FloatRect pBox = m_playerModel.getHitbox();
                float range = 50.0f;
                sf::FloatRect hitzone(
                    m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                    pBox.top, range, pBox.height
                );

                if (hitzone.intersects(m_boss.getHitbox())) {
                    m_boss.takeDamage(m_playerModel.getAttackDamage());
                    m_playerModel.m_hasDealtDamage = true;
                }
            }
        }

        // B. BOSS ATTACKS THE PLAYER
        if (m_boss.getState() == ATTACKING && !m_boss.m_hasDealtDamage) {
            if (m_boss.getStateTimer() >= 0.5f) {
                sf::FloatRect bBox = m_boss.getHitbox();
                float bRange = 60.0f;
                sf::FloatRect bHitzone(
                    m_boss.isFacingRight() ? bBox.left + bBox.width : bBox.left - bRange,
                    bBox.top, bRange, bBox.height
                );

                if (bHitzone.intersects(m_playerModel.getHitbox())) {
                    m_playerModel.takeDamage(m_boss.getAttackDamage());
                    m_boss.m_hasDealtDamage = true;
                }
            }
        }
    }

    // --- GENERIC COMBAT LOOP (Using Introspection) ---
    for (Character* enemy : m_enemies) {
        if (enemy->isDead()) continue;

        // Common Logic: Player Hits Enemy (Sword Zone Detection)
        if (m_playerModel.state == PlayerState::ATTACK && !m_playerModel.m_hasDealtDamage) {
             sf::FloatRect pBox = m_playerModel.getHitbox();
             float range = 50.0f;
             sf::FloatRect swordZone(
                m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                pBox.top, range, pBox.height
            );

            if (swordZone.intersects(enemy->getHitbox())) {
                enemy->takeDamage(m_playerModel.getAttackDamage());
                if(enemy->isDead()) {
                    int luck = rand() % 100 + 1;
                    if(luck <= 30) {
                        m_items.push_back(std::make_unique<HealthPotion>(enemy->getPosition()));
                    }
                }

                // Trigger specific hit reactions (e.g., animations)
                if (enemy->getType() == EntityType::SPIDER) {
                    SpiderModel* s = static_cast<SpiderModel*>(enemy);
                    if (s->getState() != SpiderState::DROPPING) s->setState(SpiderState::HURT);
                }
                else if (enemy->getType() == EntityType::SNAKE) {
                     static_cast<SnakeModel*>(enemy)->setState(SnakeState::HURT);
                }

                m_playerModel.m_hasDealtDamage = true;
            }
        }

        // Common Logic: Enemy Hits Player (Contact Detection)
        if (enemy->getHitbox().intersects(m_playerModel.getHitbox())) {

            bool canAttack = false;

            // Check attack readiness/cooldowns per enemy type
            if (enemy->getType() == EntityType::SNAKE) {
                canAttack = static_cast<SnakeModel*>(enemy)->canAttack();
            }
            else if (enemy->getType() == EntityType::SPIDER) {
                canAttack = static_cast<SpiderModel*>(enemy)->canAttack();
            }

            if (canAttack) {
                m_playerModel.takeDamage(enemy->getAttackDamage());

                // Reset specific attack cooldown timers
                if (enemy->getType() == EntityType::SNAKE)
                    static_cast<SnakeModel*>(enemy)->resetAttackCooldown();
                else if (enemy->getType() == EntityType::SPIDER)
                    static_cast<SpiderModel*>(enemy)->resetAttackCooldown();
            }
        }

        // Apply environment physics to enemies (Gravity and Walls)
        resolveEnemyCollision(enemy);

        // --- PLANT COMBAT LOGIC ---
        // Logic moved inside the enemy loop - checks if player gets bit by plants
        for (auto& plant : m_plants) {
            if (plant.getState() == P_ATTACKING) {
                // Precision timing: Damage only occurs at the peak of the bite animation
                if (plant.getTimer() >= 0.5f && plant.getTimer() <= 0.55f) {
                    if (plant.getBiteZone().intersects(m_playerModel.getHitbox())) {
                        m_playerModel.takeDamage(20);
                        if (m_playerModel.isDead()) {
                            m_playerModel.isEaten = true;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Player Collision Handling
 * Manages AABB collisions between the player and level walls.
 */
void Game::handleCollisions() {
    sf::Vector2f m_pPos = m_playerModel.getPosition();
    sf::Vector2f m_pVel = m_playerModel.getVelocity();
    sf::FloatRect m_pBounds = m_playerModel.getHitbox();

    std::vector<sf::FloatRect> m_nearbyWalls = m_level.getNearbyWalls(m_pPos.x, m_pPos.y);

    for (const auto& m_wall : m_nearbyWalls) {
        sf::FloatRect m_overlap;

        // Si collision détectée
        if (m_pBounds.intersects(m_wall, m_overlap)) {

            // On détermine si c'est une collision horizontale (Côté) ou verticale (Sol/Plafond)
            // On regarde quelle "tranche" de l'overlap est la plus petite
            bool m_isSideCollision = (m_overlap.width < m_overlap.height);

            // Si on dash, on ignore souvent les petits obstacles au sol, donc on force le side collision
            if (m_playerModel.isDashing) m_isSideCollision = true;

            if (m_isSideCollision) {
                // --- RÉSOLUTION HORIZONTALE ---

                // Au lieu de regarder la vitesse (m_pVel.x), on regarde la POSITION DU MUR
                // Si le centre du joueur est à GAUCHE du mur -> Le mur est à DROITE -> On pousse à GAUCHE
                if (m_pPos.x < m_wall.left) {
                    m_playerModel.setPosition(m_wall.left - m_pBounds.width / 2.0f, m_pPos.y);
                }
                // Sinon, le mur est à GAUCHE -> On pousse à DROITE
                else {
                    m_playerModel.setPosition(m_wall.left + m_wall.width + m_pBounds.width / 2.0f, m_pPos.y);
                }

                m_playerModel.setVelocity(sf::Vector2f(0.0f, m_pVel.y));

                // Annuler le dash si on tape un mur
                if (m_playerModel.isDashing) {
                    m_playerModel.isDashing = false;
                    m_playerModel.state = PlayerState::IDLE;
                }
            }
            else {
                // --- RÉSOLUTION VERTICALE (inchangée, elle était correcte) ---
                if (m_pVel.y >= 0) { // Tombe ou atterrit
                    // On vérifie qu'on est bien au-dessus du mur
                    if (m_pBounds.top < m_wall.top) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top - 0.01f);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));
                        m_playerModel.m_isGrounded = true;

                        if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {
                            m_playerModel.state = (std::abs(m_pVel.x) > 0.5f) ? PlayerState::RUN : PlayerState::IDLE;
                        }
                    }
                }
                else if (m_pVel.y < 0) { // Saute et tape la tête
                    // On vérifie qu'on est bien en dessous du mur
                    if (m_pBounds.top > m_wall.top) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top + m_wall.height + m_pBounds.height + 0.01f);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));
                    }
                }
            }
            // Mise à jour pour la prochaine itération de boucle
            m_pBounds = m_playerModel.getHitbox();
            m_pPos = m_playerModel.getPosition();
        }
    }
}

/**
 * Boss Environment Collision
 * Specifically handles the Boss's interaction with platforms.
 */
void Game::handleBossCollisions() {
    sf::Vector2f bPos = m_boss.getPosition();
    sf::Vector2f bVel = m_boss.getVelocity();
    sf::FloatRect bBounds = m_boss.getHitbox();

    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(bPos.x, bPos.y);

    for (const auto& wall : walls) {
        sf::FloatRect overlap;

        if (bBounds.intersects(wall, overlap)) {
            // On détermine le côté le moins enfoncé
            bool isSideCollision = (overlap.width < overlap.height);

            // Priorité au sol si on tombe (même si on touche un coin)
            if (bVel.y > 0 && (bBounds.top + bBounds.height - wall.top) < 15.0f) {
                isSideCollision = false;
            }

            if (isSideCollision) {
                // --- MUR ---
                if (bPos.x < wall.left) { // Pousser à gauche
                    m_boss.setPosition(wall.left - bBounds.width / 2.0f, bPos.y);
                } else { // Pousser à droite
                    m_boss.setPosition(wall.left + wall.width + bBounds.width / 2.0f, bPos.y);
                }

                // On annule juste la vitesse (Stop net) au lieu de rebondir
                // Ça évite le ping-pong infini
                m_boss.setVelocity(sf::Vector2f(0.0f, bVel.y));
            }
            else {
                // --- SOL / PLAFOND ---
                if (bVel.y >= 0) { // Sol
                    if (bBounds.top < wall.top) {
                        m_boss.setPosition(bPos.x, wall.top - 0.01f);
                        m_boss.setVelocity(sf::Vector2f(bVel.x, 0.0f));
                    }
                }
                else if (bVel.y < 0) { // Plafond
                     if (bBounds.top > wall.top) {
                        m_boss.setPosition(bPos.x, wall.top + wall.height + bBounds.height + 0.01f);
                        m_boss.setVelocity(sf::Vector2f(bVel.x, 0.0f));
                    }
                }
            }
            // Mise à jour vitale
            bBounds = m_boss.getHitbox();
            bPos = m_boss.getPosition();
        }
    }
}

/**
 * Generic Enemy Physics Resolution
 * Handles wall collisions and state transitions for all standard enemies.
 */
void Game::resolveEnemyCollision(Character* enemy) {
    // 1. Spider Specific Logic: Hanging spiders are ethereal/ignore walls
    if (enemy->getType() == EntityType::SPIDER) {
        SpiderModel* s = static_cast<SpiderModel*>(enemy);
        if (s->getState() == SpiderState::HANGING) return;
    }

    // 2. Generic Wall Collision Logic
    sf::Vector2f pos = enemy->getPosition();
    sf::Vector2f vel = enemy->getVelocity();
    sf::FloatRect bounds = enemy->getHitbox();
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(pos.x, pos.y);

    bool onGround = false;

    for (const auto& wall : walls) {
        if (bounds.intersects(wall)) {
            // Landing detection
            if (vel.y >= 0 && (bounds.top + bounds.height) < (wall.top + 30.0f)) {
                enemy->setVelocity(sf::Vector2f(vel.x, 0.0f));
                enemy->setPosition(pos.x, wall.top);
                onGround = true;
            }
        }
    }

    // 3. Spider State Transition: Transition from dropping to ground walking
    if (enemy->getType() == EntityType::SPIDER) {
        SpiderModel* s = static_cast<SpiderModel*>(enemy);
        if (s->getState() == SpiderState::DROPPING && onGround) {
            s->setState(SpiderState::WALK);
        }
    }
}

/**
 * Unified Rendering Routine
 * Handles normal gameplay, the transition screen, and the pause menu overlay.
 */
void Game::render() {
    m_window.clear(sf::Color(50, 50, 50));

    if (m_isTransitioning) {
        // Draw only the black transition screen (Mario style)
        m_window.setView(m_window.getDefaultView());
        m_transitionView.draw(m_window, m_transitionModel);
    }
    else {
        // --- DRAW GAME WORLD ---
        m_window.setView(m_camera);
        m_window.draw(m_background);
        m_window.draw(m_levelView);

        for (auto& pView : m_plantViews) {
            pView.draw(m_window);
        }

        if (m_currentLevelId == 1) {
            m_guardianView.draw(m_window, m_guardianPos);
        }

        if (m_currentLevelId == 2) m_bossView.draw(m_window);

        m_playerView.draw(m_window);
        if (!m_playerModel.isEaten) {
            m_playerView.draw(m_window);
        }

        // Enemy rendering
        int snakeIdx = 0;
        int spiderIdx = 0;
        for (Character* enemy : m_enemies) {
            if (enemy->isDead()) continue;
            if (enemy->getType() == EntityType::SNAKE) {
                m_snakeViews[snakeIdx].draw(m_window);
                snakeIdx++;
            }
            else if (enemy->getType() == EntityType::SPIDER) {
                m_spiderViews[spiderIdx].draw(m_window, *static_cast<SpiderModel*>(enemy));
                spiderIdx++;
            }
        }

        //Draw items
        for (const auto& potion : m_items) {
            // On demande à la vue de dessiner la potion à sa position actuelle
            m_healthPotionView.draw(m_window, potion->getPosition());
        }

        // --- DRAW HUD ---
        // HUD is drawn using the static default view (doesn't move with camera)
        m_hud.draw(m_window, m_playerModel.getHP(), m_playerModel.getPosition());

        // --- DRAW PAUSE MENU OVERLAY ---
        if (m_isPaused) {
            m_window.setView(m_window.getDefaultView());
            m_pauseView.draw(m_window, m_pauseModel);
        }
    }

    m_window.display();
}

/**
 * Level Transition Logic
 * Rebuilds the world state for Level 2 (The Cave).
 */
void Game::loadCaveLevel() {
    std::cout << "--- TRANSITION TO THE CAVE ---" << std::endl;

    m_currentLevelId = 2;
    m_soundManager.playCaveMusic();
    m_level.loadLevel(2);
    m_levelView.loadTileset("resources/Tileset_Cave.png");
    m_levelView.build(m_level);

    // Switch Background layers
    m_background.clearLayers();
    m_background.addLayer("resources/cave_background.png");

    // Reset positions for Level 2
    m_playerModel.setPosition(50.0f, 50.0f);
    m_playerModel.setVelocity(sf::Vector2f(0, 0));
    m_boss.reset(500.0f, 350.0f);
}
