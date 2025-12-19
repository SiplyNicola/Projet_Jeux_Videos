#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game() : m_background(sf::Vector2u(1280, 720)) {
    m_window.create(sf::VideoMode(1920, 1080), "Whisper of Steel - Final");
    m_window.setFramerateLimit(60);

    m_camera.setSize(1280, 720);

    // Initialisation des vues et du niveau
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel();
    m_levelView.build(m_level);

    // Chargement des couches du décor
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

    // Ne traite les entrées que si le joueur est vivant
    if (!m_playerModel.isDead()) {
        m_playerController.handleInput(m_playerModel);
    }
}

void Game::update(float dt) {
    // 1. Mise à jour de la physique
    m_playerModel.update(dt);
    m_boss.updateBoss(dt, m_playerModel.getPosition()); // IA du boss vers joueur

    // 2. Systèmes de jeu
    handleCollisions();
    handleBossCollisions();
    handleCombat(); // Gestion des baffes !

    // 3. Système de Respawn (Chute)
    float mapHeightPixel = m_level.getMapData().size() * 72.0f;
    if (m_playerModel.getPosition().y > mapHeightPixel + 200.0f) {
        m_playerModel.setPosition(100.0f, 2520.0f);
        m_playerModel.setVelocity(sf::Vector2f(0.0f, 0.0f));
        m_playerModel.state = PlayerState::IDLE;
    }

    // 4. Mise à jour des Vues (Animations)
    m_playerView.updateAnimation(m_playerModel, dt);
    m_bossView.update(dt, m_boss);

    // 5. Gestion Caméra (Suivi fluide avec Clamping)
    float mapWidth = m_level.getMapData()[0].size() * 72.0f;
    sf::Vector2f viewSize = m_camera.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;

    sf::Vector2f pPos = m_playerModel.getPosition();
    float camX = std::max(halfW, std::min(pPos.x, mapWidth - halfW));
    float camY = std::max(halfH, std::min(pPos.y, mapHeightPixel - halfH));

    m_camera.setCenter(camX, camY);
    m_window.setView(m_camera);

    // 6. Background (Parallaxe)
    m_background.update(m_camera.getCenter().x - halfW, m_camera.getCenter().y - halfH);
}

void Game::handleCombat() {
    // A. LE JOUEUR ATTAQUE LE BOSS
    if (m_playerModel.state == PlayerState::ATTACK && !m_playerModel.m_hasDealtDamage) {
        // Zone de frappe à 0.15s du début de l'animation
        if (m_playerModel.attackTimer > 0.15f) {
            sf::FloatRect pBox = m_playerModel.getHitbox();
            float range = 50.0f;
            sf::FloatRect hitzone(
                m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                pBox.top, range, pBox.height
            );

            if (hitzone.intersects(m_boss.getHitbox())) {
                m_boss.takeDamage(m_playerModel.getAttackDamage()); // Utilise Entity::takeDamage
                m_playerModel.m_hasDealtDamage = true;
            }
        }
    }

    // B. LE BOSS ATTAQUE LE JOUEUR
    if (m_boss.getState() == ATTACKING && !m_boss.m_hasDealtDamage) {
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

void Game::handleCollisions() {
    sf::Vector2f pPos = m_playerModel.getPosition();
    sf::Vector2f pVel = m_playerModel.getVelocity();
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(pPos.x, pPos.y);
    sf::FloatRect pBox = m_playerModel.getHitbox();

    for (const auto& wall : walls) {
        if (pBox.intersects(wall)) {
            // Collision Sol
            if (pVel.y > 0 && pBox.top + pBox.height < wall.top + 30) {
                m_playerModel.setPosition(pPos.x, wall.top);
                m_playerModel.setVelocity(sf::Vector2f(pVel.x, 0));

                if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {
                    m_playerModel.state = (std::abs(pVel.x) > 0.5f) ? PlayerState::RUN : PlayerState::IDLE;
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
    m_bossView.draw(m_window);
    m_playerView.draw(m_window);

    m_window.display();
}
