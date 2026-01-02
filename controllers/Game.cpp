#include "Game.h"
#include <iostream>
#include <cmath>

/**
 * Game Constructor
 * Initializes the window, world state, levels, and all entity lists.
 */
Game::Game(sf::RenderWindow& window)
    : m_window(window),
      m_background(sf::Vector2u(1280, 720)),
      m_isPaused(false),
      m_playerModel(),
      m_exitToMainFlag(false)
{
    // Define initial camera size (matches the background resolution)
    m_camera.setSize(1280, 720);
    m_currentLevelId = 1;

    // Initialization of views and levels logic
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel(1); // Load level 1 data
    m_levelView.build(m_level); // Build the graphical vertex array for the map
    m_hud.init();

    // Setup the NPC Guardian
    m_guardianView.init();
    m_guardianPos = sf::Vector2f(6634.0f, 432.0f);

    // Initialize Pause Menu View
    m_pauseView.init(m_window);

    // --- UNIFIED ENEMY CREATION (HEAP ALLOCATION) ---
    // Clear previous entities to ensure a clean state
    m_enemies.clear();
    m_snakeViews.clear();
    m_spiderViews.clear();

    // --- PLANT CREATION ---
    m_plants.clear();
    m_plantViews.clear();

    // Pre-defined world positions for carnivorous plants
    std::vector<sf::Vector2f> plantPositions = {
        {1669.0f, 1449.0f},
        {1500.0f, 2520.0f},
        {4561.0f, 366.0f},
        {5829.0f, 1776.0f}
    };

    // MEMORY SAFETY: Reserve space in vectors to prevent pointer invalidation
    // during push_back operations (crucial for SFML textures in views)
    m_plantViews.reserve(plantPositions.size());
    m_plants.reserve(plantPositions.size());

    for (const auto& pos : plantPositions) {
        // Create the logical plant model
        m_plants.push_back(PlantModel(pos));

        // Create and initialize the view directly within the vector
        m_plantViews.emplace_back();
        m_plantViews.back().init();
    }

    // 1. Create Snakes - Define positions across the level
    std::vector<sf::Vector2f> m_snakePositions = {
        {400.0f, 2520.0f}, {1258.0f, 2663.0f}, {1908.0f, 2520.0f}, {2961.0f, 2264.0f},
        {3780.0f, 2520.0f}, {3780.0f, 2520.0f},{7060.0f, 2230.0f},
        {7720.0f, 1780.0f},{8500.0f, 1944.0f},{6212.0f, 1871.0f},
        {5453.0f, 1655.0f},{5002.0f, 1439.0f},{2066.0f, 719.0f},
        {6680.0f, 2520.0f},{5722.0f, 2520.0f},{5054.0f, 2664.0f},
        {1153.0f, 1124.0f},{1202.0f, 1511.0f},{1357.0f, 719.0f},{4116.0f, 1008.0f}
    };

    for (const auto& pos : m_snakePositions) {
        // Dynamic Allocation (Heap) -> Stored in Character* list for polymorphism
        SnakeModel* snake = new SnakeModel(pos.x, pos.y);
        m_enemies.push_back(snake);

        // Create associated View for visual representation
        SnakeView sView; sView.init();
        m_snakeViews.push_back(sView);
    }

    // 2. Create Spiders - Define positions on the ceiling
    std::vector<sf::Vector2f> spiderPositions = {
        {1908.0f, 2400.0f},{3924.0f, 2400.0f},{4141.0f, 2400.0f},
        {5808.0f, 2400.0f},{6584.0f, 2400.0f}, {4215.0f, 1500.0f},
        {1040.0f, 1120.0f},{1117.0f, 1400.0f},{2129.0f, 1415.0f}
    };

    for (const auto& pos : spiderPositions) {
        // Dynamic Allocation for generic character handling
        SpiderModel* spider = new SpiderModel(pos.x, pos.y);
        m_enemies.push_back(spider);

        // Associated graphical view
        SpiderView spView; spView.init();
        m_spiderViews.push_back(spView);
    }

    // Load Parallax background layers
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png");

    // Level transition trigger zone
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
}

/**
 * Main Game Loop
 * Handles timing, events, world updates, and rendering.
 */
void Game::run() {
    sf::Clock m_clock;
    while (m_window.isOpen() && !m_exitToMainFlag) {
        // Restart clock to get the frame delta time
        float m_dt = m_clock.restart().asSeconds();
        processEvents();

        // Pause mechanism: Only update world logic if the game is active
        if (!m_isPaused) {
            update(m_dt);
        }

        render();

        // Death Handling: Wait a moment before exiting if player is dead
        if (m_playerModel.isDead()) {
            sf::sleep(sf::seconds(1.0f));
            return;
        }
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

        // Check for Pause Toggle (Key 'M')
        if (m_event.type == sf::Event::KeyPressed && m_event.key.code == sf::Keyboard::M) {
            m_isPaused = !m_isPaused;
            m_pauseModel.reset();
        }

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
        m_playerController.handleInput(m_playerModel);
    }
}

/**
 * Game World Update Logic
 * Updates entities, handles collisions, combat, and camera movements.
 */
void Game::update(float m_dt) {
    // 1. Update Models (Physics and Logic)
    m_playerModel.update(m_dt);

    // Boss only exists and updates in Level 2
    if (m_currentLevelId == 2) {
        m_boss.updateBoss(m_dt, m_playerModel.getPosition());
    }

    // Early exit if player died during this update frame
    if (m_playerModel.isDead()) return;

    // --- INTROSPECTION LOOP ---
    // Iterates over generic character list and applies specific logic based on entity type.
    int snakeIndex = 0;
    int spiderIndex = 0;

    for (Character* enemy : m_enemies) {
        if (enemy->isDead()) continue; // Ignore dead entities

        // Determine specific logic using runtime type identification
        switch (enemy->getType()) {

            case EntityType::SNAKE: {
                // Cast to specific model after type verification
                SnakeModel* snake = static_cast<SnakeModel*>(enemy);
                snake->update(m_dt, m_playerModel.getPosition());

                // Synchronize with the corresponding graphical view
                if (snakeIndex < m_snakeViews.size()) {
                    m_snakeViews[snakeIndex].update(m_dt, *snake);
                }
                snakeIndex++;
                break;
            }

            case EntityType::SPIDER: {
                SpiderModel* spider = static_cast<SpiderModel*>(enemy);
                spider->update(m_dt, m_playerModel.getPosition());

                if (spiderIndex < m_spiderViews.size()) {
                    m_spiderViews[spiderIndex].update(m_dt, *spider);
                }
                spiderIndex++;
                break;
            }

            default: break;
        }
    }

    // Resolve Combat, Physics, and Tile Collisions
    handleCollisions();
    if (m_currentLevelId == 2) {
        handleBossCollisions();
    }
    handleCombat();

    // Interaction zone around the NPC Guardian
    sf::FloatRect guardianZone(m_guardianPos.x - 50, m_guardianPos.y - 50, 100, 100);

    if (m_playerModel.getHitbox().intersects(guardianZone)) {
        // Interact key: 'E'
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

            if (m_playerModel.getCoins() >= 3) {
                std::cout << "Guardian: You have the 3 coins. Cross the bridge!" << std::endl;
                loadCaveLevel(); // <--- VICTORY: CHANGE LEVEL
            }
            else {
                std::cout << "Guardian: You need 3 coins!" << std::endl;
                // Repel the player slightly to prevent bypassing without coins
                m_playerModel.setPosition(m_playerModel.getPosition().x - 20, m_playerModel.getPosition().y);
            }
        }
    }

    // Update all carnivorous plants (logic + animation sync)
    for (size_t i = 0; i < m_plants.size(); i++) {
        m_plants[i].update(m_dt);
        m_plantViews[i].update(m_dt, m_plants[i]);
    }

    // Camera Clamping Logic: Keeps camera within map boundaries
    float m_mapW = m_level.getMapData()[0].size() * 72.0f;
    float m_mapH = m_level.getMapData().size() * 72.0f;
    sf::Vector2f m_pPos = m_playerModel.getPosition();
    float m_cX = std::max(640.0f, std::min(m_pPos.x, m_mapW - 640.0f));
    float m_cY = std::max(360.0f, std::min(m_pPos.y, m_mapH - 360.0f));
    m_camera.setCenter(m_cX, m_cY);

    // Update visual animations for active entities
    m_playerView.updateAnimation(m_playerModel, m_dt);
    if (m_currentLevelId == 2) {
        m_bossView.update(m_dt, m_boss);
    }

    // Background parallax movement synchronized with camera
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

    // Optimize by only fetching walls near the player's current position
    std::vector<sf::FloatRect> m_nearbyWalls = m_level.getNearbyWalls(m_pPos.x, m_pPos.y);

    for (const auto& m_wall : m_nearbyWalls) {
        sf::FloatRect m_overlap;
        if (m_pBounds.intersects(m_wall, m_overlap)) {

            // Detect if collision is mostly horizontal or vertical
            bool m_isSideCollision = (m_overlap.width < m_overlap.height) || m_playerModel.isDashing;

            if (m_isSideCollision && std::abs(m_pVel.x) > 0.01f) {
                // --- SIDE RESOLUTION ---
                if (m_pVel.x > 0) {
                    m_playerModel.setPosition(m_wall.left - m_pBounds.width / 2.0f, m_pPos.y);
                } else if (m_pVel.x < 0) {
                    m_playerModel.setPosition(m_wall.left + m_wall.width + m_pBounds.width / 2.0f, m_pPos.y);
                }

                m_playerModel.setVelocity(sf::Vector2f(0.0f, m_pVel.y));

                // Cancel dash if player hits a wall
                if (m_playerModel.isDashing) {
                    m_playerModel.isDashing = false;
                    m_playerModel.state = PlayerState::IDLE;
                }
            }
            else {
                // --- VERTICAL RESOLUTION ---
                if (m_pVel.y >= 0) { // Landing on ground or falling
                    // Allow small margin (15px) for landing detection accuracy
                    if (m_pBounds.top + m_pBounds.height <= m_wall.top + 15.0f) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top - 0.01f);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));
                        m_playerModel.m_isGrounded = true;

                        // Reset animation state upon landing
                        if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {
                            m_playerModel.state = (std::abs(m_pVel.x) > 0.5f) ? PlayerState::RUN : PlayerState::IDLE;
                        }
                    }
                }
                else if (m_pVel.y < 0) { // Hitting a ceiling
                    if (m_pBounds.top >= m_wall.top + m_wall.height - 15.0f) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top + m_wall.height + m_pBounds.height + 0.01f);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));
                    }
                }
            }
            // Update temporary bounds/position for next wall check
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
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(bPos.x, bPos.y);
    sf::FloatRect bossBox = m_boss.getHitbox();

    for (const auto& wall : walls) {
        if (bossBox.intersects(wall)) {
            float overlapY = (bossBox.top + bossBox.height) - wall.top;
            // Floor landing logic for Boss
            if (bVel.y > 0 && overlapY < 70.0f) {
                m_boss.setVelocity(sf::Vector2f(bVel.x, 0));
                m_boss.setPosition(bPos.x, wall.top - 50.0f);
            }
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
 * Rendering Routine
 * Draws all entities, backgrounds, and UI elements to the screen.
 */
void Game::render() {
    m_window.clear(sf::Color(50, 50, 50));
    m_window.setView(m_camera);

    // Draw Background, Map, and Static/Semi-static entities
    m_window.draw(m_background);
    m_window.draw(m_levelView);

    // Draw all carnivorous plants
    for (auto& pView : m_plantViews) {
        pView.draw(m_window);
    }

    // Level-specific static characters
    if (m_currentLevelId == 1) {
        m_guardianView.draw(m_window, m_guardianPos);
    }
    if (m_currentLevelId == 2) m_bossView.draw(m_window);

    // Draw Main Character and HUD
    m_playerView.draw(m_window);
    m_hud.draw(m_window, m_playerModel.getHP(), m_playerModel.getPosition());

    // Enemy Render Loop
    int snakeIdx = 0;
    int spiderIdx = 0;

    for (Character* enemy : m_enemies) {
        if (enemy->isDead()) continue;

        if (enemy->getType() == EntityType::SNAKE) {
            m_snakeViews[snakeIdx].draw(m_window);
            snakeIdx++;
        }
        else if (enemy->getType() == EntityType::SPIDER) {
            // Pass the specific model to draw the spider silk/web thread
            m_spiderViews[spiderIdx].draw(m_window, *static_cast<SpiderModel*>(enemy));
            spiderIdx++;
        }
    }

    // UI Overlay: Draw Pause menu on top if active
    if (m_isPaused) {
        m_window.setView(m_window.getDefaultView());
        m_pauseView.draw(m_window, m_pauseModel);
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
    m_level.loadLevel(2);
    m_levelView.loadTileset("resources/Tileset_Cave.png");
    m_levelView.build(m_level);

    // Switch Background layers
    m_background.clearLayers();
    m_background.addLayer("resources/cave_background.png");

    // Reset positions for Level 2
    m_playerModel.setPosition(100.0f, 300.0f);
    m_playerModel.setVelocity(sf::Vector2f(0, 0));
    m_boss.reset(600.0f, 400.0f);
}
