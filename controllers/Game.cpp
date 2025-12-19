#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game() : m_background(sf::Vector2u(1280, 720)) {
    m_window.create(sf::VideoMode(1920, 1080), "Whisper of Steel - Final");
    m_window.setFramerateLimit(60);

    m_camera.setSize(1280, 720);

    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel();
    m_levelView.build(m_level);

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
    m_playerController.handleInput(m_playerModel);
}

void Game::update(float dt) {
    // 1. Mise à jour Physique
    m_playerModel.update(dt);
    // MODIF : Utilisation de updateBoss et getPosition()
    m_boss.updateBoss(dt, m_playerModel.getPosition());

    // 2. Collisions
    handleCollisions();
    handleBossCollisions();

    // 3. Système de Respawn
    float mapHeightPixel = m_level.getMapData().size() * 72.0f;
    // MODIF : getPosition().y remplace position.y
    if (m_playerModel.getPosition().y > mapHeightPixel + 200.0f) {
        // MODIF : Utilisation des setters
        m_playerModel.setPosition(100.0f, 2520.0f);
        m_playerModel.setVelocity(sf::Vector2f(0.0f, 0.0f));
        m_playerModel.state = PlayerState::IDLE;
    }

    // 4. Mise à jour des Vues
    m_playerView.updateAnimation(m_playerModel, dt);
    m_bossView.update(dt, m_boss);

    // 5. Gestion Caméra
    float mapWidth = m_level.getMapData()[0].size() * 72.0f;
    sf::Vector2f viewSize = m_camera.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;

    // MODIF : getPosition()
    float camX = m_playerModel.getPosition().x;
    float camY = m_playerModel.getPosition().y;

    if (camX < halfW) camX = halfW;
    if (camX > mapWidth - halfW) camX = mapWidth - halfW;
    if (camY < halfH) camY = halfH;
    if (camY > mapHeightPixel - halfH) camY = mapHeightPixel - halfH;

    m_camera.setCenter(camX, camY);
    m_window.setView(m_camera);

    // 6. Background
    float camLeft = m_camera.getCenter().x - halfW;
    float camTop  = m_camera.getCenter().y - halfH;
    m_background.update(camLeft, camTop);
}

void Game::handleCollisions() {
    // MODIF : Récupération des données via getters
    sf::Vector2f pPos = m_playerModel.getPosition();
    sf::Vector2f pVel = m_playerModel.getVelocity();

    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(pPos.x, pPos.y);
    sf::FloatRect pBox = m_playerModel.getHitbox();

    for (const auto& wall : walls) {
        if (pBox.intersects(wall)) {
            // Collision SOL : on vérifie si on tombe et si on est au-dessus du mur
            if (pVel.y > 0 && pBox.top + pBox.height < wall.top + 30) {
                // MODIF : Utilisation des setters pour corriger la physique
                m_playerModel.setPosition(pPos.x, wall.top);
                m_playerModel.setVelocity(sf::Vector2f(pVel.x, 0));

                if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {
                    if (std::abs(pVel.x) > 0.5f) {
                        m_playerModel.state = PlayerState::RUN;
                    } else {
                        m_playerModel.state = PlayerState::IDLE;
                    }
                }
            }
        }
    }
}

void Game::handleBossCollisions() {
    // Utilise déjà les getters/setters définis dans BossModel
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(m_boss.getPosition().x, m_boss.getPosition().y);
    sf::FloatRect bossBox = m_boss.getHitbox();

    for (const auto& wall : walls) {
        if (bossBox.intersects(wall)) {
            float overlapY = (bossBox.top + bossBox.height) - wall.top;
            if (m_boss.getVelocity().y > 0 && overlapY < 70.0f) {
                m_boss.setVelocity(sf::Vector2f(m_boss.getVelocity().x, 0));
                m_boss.setPosition(m_boss.getPosition().x, wall.top - 50.0f);
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
