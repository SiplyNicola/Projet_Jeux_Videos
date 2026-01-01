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

    // Snake setup
    std::vector<sf::Vector2f> m_pos = {{400.0f, 2520.0f}, {600.0f, 2520.0f}, {800.0f, 2520.0f}, {1200.0f, 2450.0f}};
    for (const auto& p : m_pos) {
        m_snakes.emplace_back(p.x, p.y);
        SnakeView m_sView;
        m_sView.init();
        m_snakeViews.push_back(m_sView);
    }

    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png");
}

Game::~Game() {
    // Destructor implementation
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

    m_playerModel.update(m_dt);
    m_boss.updateBoss(m_dt, m_playerModel.getPosition());
    m_plantModel.update(m_dt);

    if (m_playerModel.isDead()) return;

    for (auto& m_s : m_snakes) m_s.update(m_dt, m_playerModel.getPosition());

    // Combat & Collisions
    handleCollisions();
    handleBossCollisions();
    handleCombat();
    handleSnakeCollisions();

    // Camera Clamping logic
    float m_mapW = m_level.getMapData()[0].size() * 72.0f;
    float m_mapH = m_level.getMapData().size() * 72.0f;
    sf::Vector2f m_pPos = m_playerModel.getPosition();
    float m_cX = std::max(640.0f, std::min(m_pPos.x, m_mapW - 640.0f));
    float m_cY = std::max(360.0f, std::min(m_pPos.y, m_mapH - 360.0f));
    m_camera.setCenter(m_cX, m_cY);

    m_plantView.update(m_dt, m_plantModel);
    m_playerView.updateAnimation(m_playerModel, m_dt);
    m_bossView.update(m_dt, m_boss);
    for (size_t i = 0; i < m_snakes.size(); i++) m_snakeViews[i].update(m_dt, m_snakes[i]);

    m_background.update(m_camera.getCenter().x - 640.0f, m_camera.getCenter().y - 360.0f);
}
void Game::handleCombat() {
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

    // B. GESTION DES SERPENTS (BOUCLE)
    for (auto& snake : m_snakes) {

        // On ignore les morts
        if (snake.getState() == SnakeState::DEATH) continue;

        // 1. JOUEUR FRAPPE SERPENT
        if (m_playerModel.state == PlayerState::ATTACK && !m_playerModel.m_hasDealtDamage) {
            sf::FloatRect pBox = m_playerModel.getHitbox();
            float range = 50.0f;
            sf::FloatRect swordZone(
                m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                pBox.top, range, pBox.height
            );

            // On vérifie si l'épée touche CE serpent spécifique
            if (swordZone.intersects(snake.getHitbox())) {
                snake.takeDamage(10);
                snake.setState(SnakeState::HURT);
                if (snake.getHP() <= 0) snake.setState(SnakeState::DEATH);
                m_playerModel.m_hasDealtDamage = true;
            }
        }

        // 2. SERPENT MORD JOUEUR
        if (snake.getState() != SnakeState::HURT &&
            m_playerModel.getHitbox().intersects(snake.getHitbox())) {

            if (snake.canAttack()) {
                m_playerModel.takeDamage(1);
                snake.resetAttackCooldown();

            }
        }
    } // <--- C'est ici qu'on ferme la boucle des serpents !

    // C. LE BOSS ATTAQUE LE JOUEUR (Maintenant c'est bien séparé)
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
                        // SNAP: Use a tiny offset to keep the player just above the surface
                        // This prevents the "jitter" on the next frame
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
            // Update local variables for the next wall check in the loop
            m_pBounds = m_playerModel.getHitbox();
            m_pPos = m_playerModel.getPosition();
        }
    }
}

void Game::handleSnakeCollisions() {
    // On applique TA logique à CHAQUE serpent de la liste
    for (auto& snake : m_snakes) {

        // On ignore les morts pour la physique (optionnel mais conseillé)
        if (snake.getState() == SnakeState::DEATH) continue;

        // --- TON CODE EXACT (juste m_snakeModel remplacé par 'snake') ---
        sf::Vector2f sPos = snake.getPosition();
        sf::Vector2f sVel = snake.getVelocity();
        std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(sPos.x, sPos.y);
        sf::FloatRect snakeBox = snake.getHitbox();

        for (const auto& wall : walls) {
            if (snakeBox.intersects(wall)) {
                if (sVel.y > 0 && (snakeBox.top + snakeBox.height) < (wall.top + 30.0f)) {
                    snake.setVelocity(sf::Vector2f(sVel.x, 0));
                    snake.setPosition(sPos.x, wall.top);
                }
            }
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

void Game::render() {
    m_window.clear(sf::Color(50, 50, 50));
    m_window.setView(m_camera);

    m_window.draw(m_background);
    m_window.draw(m_levelView);
    m_plantView.draw(m_window);
    m_bossView.draw(m_window);
    m_playerView.draw(m_window);
    m_hud.draw(m_window, m_playerModel.getHP(), m_playerModel.getPosition());

    for (size_t i = 0; i < m_snakes.size(); i++) {
        if (m_snakes[i].getState() != SnakeState::DEATH) m_snakeViews[i].draw(m_window);
    }

    // --- PAUSE OVERLAY ---
    if (m_isPaused) {
        // Switch to default UI view so menu doesn't move with camera
        m_window.setView(m_window.getDefaultView());
        m_pauseView.draw(m_window, m_pauseModel);
    }

    m_window.display();
}

void Game::loadCaveLevel() {
    std::cout << "--- TRANSITION VERS LA GROTTE ---" << std::endl;

    m_currentLevelId = 2;

    // 1. Changer les donn�es du niveau
    m_level.loadLevel(2);     // Charge le tableau de la grotte (dans LevelModel)

    m_levelView.loadTileset("resources/Tileset_Cave.png");

    m_levelView.build(m_level); // Reconstruit l'image (LevelView)

    // 2. Changer le Background (On suppose que tu as ajout� clearLayers dans Background.h)
    // m_background.clearLayers();
    // m_background.addLayer("resources/cave_bg.png"); // Si tu as une image

    // 3. Replacer le joueur au d�but de la grotte
    m_playerModel.setPosition(100.0f, 300.0f);
    m_playerModel.setVelocity(sf::Vector2f(0, 0));

    // 4. Placer le Boss (Il t'attend plus loin dans la grotte)
    m_boss.setPosition(600.0f, 400.0f);
}
