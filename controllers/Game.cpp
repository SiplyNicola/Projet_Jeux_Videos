#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game() : m_background(sf::Vector2u(1280, 720)), m_plantModel({100.0f, 2520.0f}) {
    m_window.create(sf::VideoMode(1920, 1080), "Whisper of Steel - Final");
    m_window.setFramerateLimit(60);
    m_camera.setSize(1280, 720);

    // Initialisation
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel();
    m_levelView.build(m_level);
    m_plantView.init();
    m_snakeView.init();
    m_hud.init();

    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png");
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) m_window.close();
    }

    // On ne peut plus bouger si on est mort
    if (!m_playerModel.isDead()) {
        m_playerController.handleInput(m_playerModel);
    }
}

void Game::update(float dt) {
    m_plantModel.update(dt);
    m_plantView.update(dt, m_plantModel); // Note: déjà mis à jour plus bas dans 'Mise à jour des vues'

    // 2. VÉRIFICATION DE LA MORT DU JOUEUR
    if (m_playerModel.isDead()) return;

    // 3. MISE À JOUR DU JOUEUR, DU BOSS ET DU SERPENT
    m_playerModel.update(dt);
    m_boss.updateBoss(dt, m_playerModel.getPosition());
    m_snakeModel.update(dt, m_playerModel.getPosition());

    // --- LOGIQUE DE MORT PAR LA PLANTE ---
    if (m_plantModel.getState() == P_ATTACKING) {
        if (m_playerModel.getHitbox().intersects(m_plantModel.getBiteZone())) {
            m_playerModel.takeDamage(999);
            m_playerModel.state = PlayerState::DEAD;
        }
    }

    // 2. Traitement des interactions (Collisions & Combat)
    handleCollisions();
    handleBossCollisions();
    handleCombat();
    handleSnakeCollisions();

    // 3. Système de Respawn (Si chute dans le vide)
    float mapHeightPixel = m_level.getMapData().size() * 72.0f;
    if (m_playerModel.getPosition().y > mapHeightPixel + 200.0f) {
        m_playerModel.setPosition(100.0f, 2520.0f);
        m_playerModel.setVelocity(sf::Vector2f(0.0f, 0.0f));
        m_playerModel.state = PlayerState::IDLE;
    }

    // 4. Mise à jour des vues
    m_plantView.update(dt, m_plantModel);
    m_playerView.updateAnimation(m_playerModel, dt);
    m_bossView.update(dt, m_boss);

    // CORRECTION ICI : On utilise bien m_snakeModel
    m_snakeView.update(dt, m_snakeModel);

    // 5. Caméra avec clamping
    float mapWidth = m_level.getMapData()[0].size() * 72.0f;
    sf::Vector2f viewSize = m_camera.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;

    sf::Vector2f pPos = m_playerModel.getPosition();
    float camX = std::max(halfW, std::min(pPos.x, mapWidth - halfW));
    float camY = std::max(halfH, std::min(pPos.y, mapHeightPixel - halfH));

    m_camera.setCenter(camX, camY);
    m_window.setView(m_camera);

    // 6. Background
    m_background.update(m_camera.getCenter().x - halfW, m_camera.getCenter().y - halfH);
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

    // B. GESTION DU SERPENT
    if (m_snakeModel.getState() != SnakeState::DEATH) {

        // 1. JOUEUR TAPPE SERPENT (Attaque Épée)
        if (m_playerModel.state == PlayerState::ATTACK && !m_playerModel.m_hasDealtDamage) {
            sf::FloatRect pBox = m_playerModel.getHitbox();
            float range = 50.0f;
            sf::FloatRect swordZone(
                m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                pBox.top, range, pBox.height
            );

            if (swordZone.intersects(m_snakeModel.getHitbox())) {
                m_snakeModel.takeDamage(10);
                m_snakeModel.setState(SnakeState::HURT);
                if (m_snakeModel.getHP() <= 0) m_snakeModel.setState(SnakeState::DEATH);
                m_playerModel.m_hasDealtDamage = true;
            }
        }

        // 2. SERPENT MORD JOUEUR (Avec Délai et Ejection)
        if (m_snakeModel.getState() != SnakeState::HURT &&
            m_playerModel.getHitbox().intersects(m_snakeModel.getHitbox())) {

            // On vérifie si le serpent peut attaquer (Cooldown fini ?)
            if (m_snakeModel.canAttack()) {

                m_playerModel.takeDamage(1);
                m_snakeModel.resetAttackCooldown(); // Reset du timer d'attaque
            }
        }
    }

    // C. LE BOSS ATTAQUE LE JOUEUR
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

    // Fetch potential collision tiles
    std::vector<sf::FloatRect> m_nearbyWalls = m_level.getNearbyWalls(m_pPos.x, m_pPos.y);

    for (const auto& m_wall : m_nearbyWalls) {
        sf::FloatRect m_overlap;
        if (m_pBounds.intersects(m_wall, m_overlap)) {

            // 1. Determine collision type based on intersection shape
            bool m_isSideCollision = (m_overlap.width < m_overlap.height) || m_playerModel.isDashing;

            if (m_isSideCollision && std::abs(m_pVel.x) > 0.01f) {
                // --- SIDE COLLISIONS (Walls) ---
                if (m_pVel.x > 0) m_playerModel.setPosition(m_wall.left - m_pBounds.width / 2.0f, m_pPos.y);
                else if (m_pVel.x < 0) m_playerModel.setPosition(m_wall.left + m_wall.width + m_pBounds.width / 2.0f, m_pPos.y);

                m_playerModel.setVelocity(sf::Vector2f(0.0f, m_pVel.y));

                if (m_playerModel.isDashing) {
                    m_playerModel.isDashing = false;
                    m_playerModel.state = PlayerState::IDLE;
                }
                // CRITICAL: We do NOT set m_isGrounded here
            }
            else {
                // --- VERTICAL COLLISIONS ---
                if (m_pVel.y >= 0) { // Moving down or falling
                    // STRICT CHECK: Only ground the player if their feet are near the top of the block
                    if (m_pBounds.top + m_pBounds.height <= m_wall.top + 15.0f) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));

                        m_playerModel.m_isGrounded = true; // Now jump is enabled

                        if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {
                            m_playerModel.state = (std::abs(m_pVel.x) > 0.5f) ? PlayerState::RUN : PlayerState::IDLE;
                        }
                    }
                }
                else if (m_pVel.y < 0) { // Hitting a Ceiling
                    // Ensure we only snap to ceiling if we are actually below the block [cite: 810]
                    if (m_pBounds.top >= m_wall.top + m_wall.height - 15.0f) {
                        m_playerModel.setPosition(m_pPos.x, m_wall.top + m_wall.height + m_pBounds.height);
                        m_playerModel.setVelocity(sf::Vector2f(m_pVel.x, 0.0f));
                    }
                }
            }
            // Update local variables for the next wall check in this frame
            m_pBounds = m_playerModel.getHitbox();
            m_pPos = m_playerModel.getPosition();
        }
    }
}

void Game::handleSnakeCollisions() {
    sf::Vector2f sPos = m_snakeModel.getPosition();
    sf::Vector2f sVel = m_snakeModel.getVelocity();
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(sPos.x, sPos.y);
    sf::FloatRect snakeBox = m_snakeModel.getHitbox();

    for (const auto& wall : walls) {
        if (snakeBox.intersects(wall)) {
            if (sVel.y > 0 && (snakeBox.top + snakeBox.height) < (wall.top + 30.0f)) {
                m_snakeModel.setVelocity(sf::Vector2f(sVel.x, 0));
                m_snakeModel.setPosition(sPos.x, wall.top);
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

    // DESSINE LE SERPENT SEULEMENT S'IL EST VIVANT
    if (m_snakeModel.getState() != SnakeState::DEATH) {
        m_snakeView.draw(m_window);
    }

    if (m_playerModel.state != PlayerState::DEAD) {
        m_playerView.draw(m_window);
    }

    m_hud.draw(m_window, m_playerModel.getHP());
    m_window.display();
}
