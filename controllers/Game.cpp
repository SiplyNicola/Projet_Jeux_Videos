#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game(sf::RenderWindow& window)
    : m_window(window),
      m_background(sf::Vector2u(1280, 720)),
      m_plantModel({100.0f, 2520.0f}),
      m_isPaused(false),
      m_exitToMainFlag(false)
{
    m_camera.setSize(1280, 720);
    m_currentLevelId = 1;

    // Initialization of views and levels
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel(1);
    m_levelView.build(m_level);
    m_plantView.init();
    m_hud.init();

    // Initialize Pause View
    m_pauseView.init(m_window);

    // --- UNIFIED ENEMY CREATION (HEAP ALLOCATION) ---
    m_enemies.clear();
    m_snakeViews.clear();
    m_spiderViews.clear();

    // 1. Create Snakes
    std::vector<sf::Vector2f> m_snakePositions = {
        {400.0f, 2520.0f}, {1258.0f, 2663.0f}, {1908.0f, 2520.0f}, {2961.0f, 2264.0f},
        {3780.0f, 2520.0f}, {3780.0f, 2520.0f},{7060.0f, 2230.0f},
        {7720.0f, 1780.0f},{8500.0f, 1944.0f},{6212.0f, 1871.0f},
        {5453.0f, 1655.0f},{5002.0f, 1439.0f},{2066.0f, 719.0f},
        {6680.0f, 2520.0f},{5722.0f, 2520.0f},{5054.0f, 2664.0f},
        {1153.0f, 1124.0f},{1202.0f, 1511.0f},{1357.0f, 719.0f},{4116.0f, 1008.0f}
    };

    for (const auto& pos : m_snakePositions) {
        // Dynamic Allocation (Heap) -> Stored in Character* list
        SnakeModel* snake = new SnakeModel(pos.x, pos.y);
        m_enemies.push_back(snake);

        // Create associated View
        SnakeView sView; sView.init();
        m_snakeViews.push_back(sView);
    }

    // 2. Create Spiders
    // Positions en HAUTEUR (Plafond).
    std::vector<sf::Vector2f> spiderPositions = {
        {1908.0f, 2400.0f},{3924.0f, 2400.0f},{4141.0f, 2400.0f},
        {5808.0f, 2400.0f},{6584.0f, 2400.0f}, {4215.0f, 1500.0f},
        {1040.0f, 1120.0f},{1117.0f, 1400.0f},{2129.0f, 1415.0f}
    };

    for (const auto& pos : spiderPositions) {
        // Dynamic Allocation
        SpiderModel* spider = new SpiderModel(pos.x, pos.y);
        m_enemies.push_back(spider);

        SpiderView spView; spView.init();
        m_spiderViews.push_back(spView);
    }

    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png");

    m_nextLevelTrigger = sf::FloatRect(3000.0f, 0.0f, 100.0f, 1000.0f);
}

Game::~Game() {
    // Justification: We manually allocated memory with 'new',
    // so we must delete it to prevent memory leaks.
    for (Character* enemy : m_enemies) {
        delete enemy;
    }
    m_enemies.clear();
}

void Game::run() {
    sf::Clock m_clock;
    while (m_window.isOpen() && !m_exitToMainFlag) {
        float m_dt = m_clock.restart().asSeconds();
        processEvents();

        // Only update game world if NOT paused
        if (!m_isPaused) {
            update(m_dt);
        }

        render();

        if (m_playerModel.isDead()) {
            sf::sleep(sf::seconds(1.0f));
            return;
        }
    }
}

void Game::processEvents() {
    sf::Event m_event;
    while (m_window.pollEvent(m_event)) {
        if (m_event.type == sf::Event::Closed) m_window.close();

        // Check for Pause Toggle
        if (m_event.type == sf::Event::KeyPressed && m_event.key.code == sf::Keyboard::M) {
            m_isPaused = !m_isPaused;
            m_pauseModel.reset();
        }

        // Delegate input to Pause Controller if active
        if (m_isPaused) {
            m_pauseController.handleInput(m_window, m_event, m_pauseModel, m_pauseView);

            // Check for specific menu actions
            if (m_pauseModel.shouldResume()) {
                m_isPaused = false;
                m_pauseModel.reset();
            }
            if (m_pauseModel.shouldExitToMain()) {
                m_exitToMainFlag = true;
            }
        }
    }

    // Player controls only active when unpaused
    if (!m_playerModel.isDead() && !m_isPaused) {
        m_playerController.handleInput(m_playerModel);
    }
}

void Game::update(float m_dt) {
    if (m_currentLevelId == 1 && m_playerModel.getHitbox().intersects(m_nextLevelTrigger)) {
        loadCaveLevel();
        return;
    }

    // 1. Mise à jour des modèles
    m_playerModel.update(m_dt);

    // On update le Boss SEULEMENT si on est niveau 2
    if (m_currentLevelId == 2) {
        m_boss.updateBoss(m_dt, m_playerModel.getPosition());
    }

    m_plantModel.update(m_dt);

    if (m_playerModel.isDead()) return;

    // --- INTROSPECTION LOOP ---
    // Justification: We iterate over the generic list and use the Type ID
    // to determine which specific logic and view update to apply.

    int snakeIndex = 0;
    int spiderIndex = 0;

    for (Character* enemy : m_enemies) {
        if (enemy->isDead()) continue; // Skip dead entities

        // INTROSPECTION: Switching based on runtime type
        switch (enemy->getType()) {

            case EntityType::SNAKE: {
                // Downcast (static_cast is safe here because we checked getType)
                SnakeModel* snake = static_cast<SnakeModel*>(enemy);
                snake->update(m_dt, m_playerModel.getPosition()); // Correction: utilisation de m_dt

                // Update associated View
                if (snakeIndex < m_snakeViews.size()) {
                    m_snakeViews[snakeIndex].update(m_dt, *snake); // Correction: utilisation de m_dt
                }
                snakeIndex++;
                break;
            }

            case EntityType::SPIDER: {
                SpiderModel* spider = static_cast<SpiderModel*>(enemy);
                spider->update(m_dt, m_playerModel.getPosition()); // Correction: utilisation de m_dt

                if (spiderIndex < m_spiderViews.size()) {
                    m_spiderViews[spiderIndex].update(m_dt, *spider); // Correction: utilisation de m_dt
                }
                spiderIndex++;
                break;
            }

            default: break;
        }
    }

    // Combat & Collisions
    handleCollisions();
    // Collisions Boss seulement au niveau 2
    if (m_currentLevelId == 2) {
        handleBossCollisions();
    }
    handleCombat();

    // NOTE: Les fonctions handleSnakeCollisions() et handleSpiderCollisions()
    // ne sont plus appelées ici car la physique est gérée dans handleCombat
    // via resolveEnemyCollision pour tous les ennemis.

    // Camera Clamping logic
    float m_mapW = m_level.getMapData()[0].size() * 72.0f;
    float m_mapH = m_level.getMapData().size() * 72.0f;
    sf::Vector2f m_pPos = m_playerModel.getPosition();
    float m_cX = std::max(640.0f, std::min(m_pPos.x, m_mapW - 640.0f));
    float m_cY = std::max(360.0f, std::min(m_pPos.y, m_mapH - 360.0f));
    m_camera.setCenter(m_cX, m_cY);

    m_plantView.update(m_dt, m_plantModel);
    m_playerView.updateAnimation(m_playerModel, m_dt);
    if (m_currentLevelId == 2) {
        m_bossView.update(m_dt, m_boss);
    }

    // Le background se déplace avec la caméra
    m_background.update(m_camera.getCenter().x - 640.0f, m_camera.getCenter().y - 360.0f);
}

void Game::handleCombat() {
    if (m_currentLevelId == 2) {
        // A. LE JOUEUR ATTAQUE LE BOSS
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

        // B. LE BOSS ATTAQUE LE JOUEUR
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

        // Common Logic: Player Hits Enemy
        if (m_playerModel.state == PlayerState::ATTACK && !m_playerModel.m_hasDealtDamage) {
             sf::FloatRect pBox = m_playerModel.getHitbox();
             float range = 50.0f;
             sf::FloatRect swordZone(
                m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                pBox.top, range, pBox.height
            );

            if (swordZone.intersects(enemy->getHitbox())) {
                enemy->takeDamage(m_playerModel.getAttackDamage());

                // Introspection for specific reactions (e.g. Spider HURT animation)
                if (enemy->getType() == EntityType::SPIDER) {
                    SpiderModel* s = static_cast<SpiderModel*>(enemy);
                    // Check logic to avoid interrupting dropping
                    if (s->getState() != SpiderState::DROPPING) s->setState(SpiderState::HURT);
                }
                else if (enemy->getType() == EntityType::SNAKE) {
                     static_cast<SnakeModel*>(enemy)->setState(SnakeState::HURT);
                }

                // Generic Death Handling could go here if managed by Character
                m_playerModel.m_hasDealtDamage = true;
            }
        }

        // Common Logic: Enemy Hits Player
        if (enemy->getHitbox().intersects(m_playerModel.getHitbox())) {

            // We need to check specific attack conditions (cooldowns)
            bool canAttack = false;

            if (enemy->getType() == EntityType::SNAKE) {
                canAttack = static_cast<SnakeModel*>(enemy)->canAttack();
            }
            else if (enemy->getType() == EntityType::SPIDER) {
                canAttack = static_cast<SpiderModel*>(enemy)->canAttack();
            }

            if (canAttack) {
                m_playerModel.takeDamage(enemy->getAttackDamage());

                // Reset cooldown
                if (enemy->getType() == EntityType::SNAKE)
                    static_cast<SnakeModel*>(enemy)->resetAttackCooldown();
                else if (enemy->getType() == EntityType::SPIDER)
                    static_cast<SpiderModel*>(enemy)->resetAttackCooldown();
            }
        }

        // Handling Enemy Physics (Walls)
        resolveEnemyCollision(enemy);
    }
}

void Game::handleCollisions() {
    sf::Vector2f m_pPos = m_playerModel.getPosition();
    sf::Vector2f m_pVel = m_playerModel.getVelocity();
    sf::FloatRect m_pBounds = m_playerModel.getHitbox();

    // Fetch nearby wall AABBs
    std::vector<sf::FloatRect> m_nearbyWalls = m_level.getNearbyWalls(m_pPos.x, m_pPos.y);

    for (const auto& m_wall : m_nearbyWalls) {
        sf::FloatRect m_overlap;
        if (m_pBounds.intersects(m_wall, m_overlap)) {

            // 1. Determine collision priority based on intersection shape
            bool m_isSideCollision = (m_overlap.width < m_overlap.height) || m_playerModel.isDashing;

            if (m_isSideCollision && std::abs(m_pVel.x) > 0.01f) {
                // --- SIDE RESOLUTION ---
                if (m_pVel.x > 0) {
                    m_playerModel.setPosition(m_wall.left - m_pBounds.width / 2.0f, m_pPos.y);
                } else if (m_pVel.x < 0) {
                    m_playerModel.setPosition(m_wall.left + m_wall.width + m_pBounds.width / 2.0f, m_pPos.y);
                }

                m_playerModel.setVelocity(sf::Vector2f(0.0f, m_pVel.y));

                if (m_playerModel.isDashing) {
                    m_playerModel.isDashing = false;
                    m_playerModel.state = PlayerState::IDLE;
                }
            }
            else {
                // --- VERTICAL RESOLUTION ---
                if (m_pVel.y >= 0) { // Landing or falling
                    // Check if feet are near the top of the block
                    if (m_pBounds.top + m_pBounds.height <= m_wall.top + 15.0f) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top - 0.01f);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));
                        m_playerModel.m_isGrounded = true;

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
            m_pBounds = m_playerModel.getHitbox();
            m_pPos = m_playerModel.getPosition();
        }
    }
}


void Game::handleBossCollisions() {
    sf::Vector2f bPos = m_boss.getPosition();
    sf::Vector2f bVel = m_boss.getVelocity();
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(bPos.x, bPos.y);
    sf::FloatRect bossBox = m_boss.getHitbox();

    for (const auto& wall : walls) {
        if (bossBox.intersects(wall)) {
            float overlapY = (bossBox.top + bossBox.height) - wall.top;
            if (bVel.y > 0 && overlapY < 70.0f) {
                m_boss.setVelocity(sf::Vector2f(bVel.x, 0));
                m_boss.setPosition(bPos.x, wall.top - 50.0f);
            }
        }
    }
}

// --- IMPLEMENTATION COMPLETE DE LA FONCTION DEMANDÉE ---
void Game::resolveEnemyCollision(Character* enemy) {
    // 1. Spider Specific: Hanging spiders ignore walls
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
            // Collision: Landing on top of a block
            if (vel.y >= 0 && (bounds.top + bounds.height) < (wall.top + 30.0f)) {
                // Snap to top
                enemy->setVelocity(sf::Vector2f(vel.x, 0.0f));
                enemy->setPosition(pos.x, wall.top);
                onGround = true;
            }
        }
    }

    // 3. Spider Transition: From Dropping to Walking
    if (enemy->getType() == EntityType::SPIDER) {
        SpiderModel* s = static_cast<SpiderModel*>(enemy);
        // If it was dropping and hit the ground, it becomes a walker
        if (s->getState() == SpiderState::DROPPING && onGround) {
            s->setState(SpiderState::WALK);
        }
    }
}

void Game::render() {
    m_window.clear(sf::Color(50, 50, 50));
    m_window.setView(m_camera);
    m_window.draw(m_background);
    m_window.draw(m_levelView);
    m_plantView.draw(m_window);
    if (m_currentLevelId == 2) m_bossView.draw(m_window);

    m_playerView.draw(m_window);
    m_hud.draw(m_window, m_playerModel.getHP(), m_playerModel.getPosition());

    // Rendu des Ennemis
    int snakeIdx = 0;
    int spiderIdx = 0;

    for (Character* enemy : m_enemies) {
        if (enemy->isDead()) continue;

        if (enemy->getType() == EntityType::SNAKE) {
            m_snakeViews[snakeIdx].draw(m_window);
            snakeIdx++;
        }
        else if (enemy->getType() == EntityType::SPIDER) {
            // On passe le modèle spécifique pour dessiner le fil
            m_spiderViews[spiderIdx].draw(m_window, *static_cast<SpiderModel*>(enemy));
            spiderIdx++;
        }
    }

    if (m_isPaused) {
        m_window.setView(m_window.getDefaultView());
        m_pauseView.draw(m_window, m_pauseModel);
    }
    m_window.display();
}

void Game::loadCaveLevel() {
    std::cout << "--- TRANSITION VERS LA GROTTE ---" << std::endl;

    m_currentLevelId = 2;
    m_level.loadLevel(2);
    m_levelView.loadTileset("resources/Tileset_Cave.png");
    m_levelView.build(m_level);
    m_background.clearLayers();
    m_background.addLayer("resources/cave_background.png");

    m_playerModel.setPosition(100.0f, 300.0f);
    m_playerModel.setVelocity(sf::Vector2f(0, 0));
    m_boss.reset(600.0f, 400.0f);
}
