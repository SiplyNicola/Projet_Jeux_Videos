#include "Game.h"
#include "HudView.h"
#include <iostream>
#include <cmath>

Game::Game() : m_background(sf::Vector2u(1280, 720)) {
    m_window.create(sf::VideoMode(1920, 1080), "Whisper of Steel - Final");
    m_window.setFramerateLimit(60);
    m_camera.setSize(1280, 720);

    m_currentLevelId = 1; // On commence niveau 1

    // Initialisation
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel(1); // <--- On charge explicitement le niveau 1
    m_levelView.build(m_level);
    m_hud.init();

    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png");
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png");

    // DEFINE LA PORTE : À la fin de ta map forêt (ajuste le X selon ta map)
    // Ici je mets X=3000 (fin supposée) et une boite haute
    m_nextLevelTrigger = sf::FloatRect(3000.0f, 0.0f, 100.0f, 1000.0f);
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

    // --- DETECTION CHANGEMENT DE NIVEAU ---
    if (m_currentLevelId == 1) {
        // Si le joueur touche la porte invisible
        if (m_playerModel.getHitbox().intersects(m_nextLevelTrigger)) {
            loadCaveLevel();
            return; // On arrête l'update pour cette frame pour éviter les bugs
        }
    }

    // 1. Mise à jour des modèles
    m_playerModel.update(dt);
    // On passe la position du joueur à l'IA du boss
    m_boss.updateBoss(dt, m_playerModel.getPosition());

    // 2. Traitement des interactions
    handleCollisions();
    handleBossCollisions();
    handleCombat();

    // 3. Système de Respawn (Si chute dans le vide)
    float mapHeightPixel = m_level.getMapData().size() * 72.0f;
    if (m_playerModel.getPosition().y > mapHeightPixel + 200.0f) {
        m_playerModel.setPosition(100.0f, 2520.0f); //
        m_playerModel.setVelocity(sf::Vector2f(0.0f, 0.0f));
        m_playerModel.state = PlayerState::IDLE;
    }

    // 4. Mise à jour des vues
    m_playerView.updateAnimation(m_playerModel, dt);
    m_bossView.update(dt, m_boss);

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
        // Le joueur frappe rapidement (déclenchement à 0.1s d'animation)
        if (m_playerModel.attackTimer > 0.1f) {
            sf::FloatRect pBox = m_playerModel.getHitbox();
            float range = 50.0f;
            sf::FloatRect hitzone(
                m_playerModel.isFacingRight() ? pBox.left + pBox.width : pBox.left - range,
                pBox.top, range, pBox.height
            );

            if (hitzone.intersects(m_boss.getHitbox())) {
                m_boss.takeDamage(m_playerModel.getAttackDamage()); //
                m_playerModel.m_hasDealtDamage = true;
            }
        }
    }

    // B. LE BOSS ATTAQUE LE JOUEUR (AVEC DÉLAI D'ANTICIPATION)
    if (m_boss.getState() == ATTACKING && !m_boss.m_hasDealtDamage) {

        // DÉLAI DE RÉACTION : On n'inflige les dégâts qu'après 0.5s d'animation
        // Cela permet au joueur de frapper puis de dasher loin.
        if (m_boss.getStateTimer() >= 0.5f) {
            sf::FloatRect bBox = m_boss.getHitbox();
            float bRange = 60.0f;
            sf::FloatRect bHitzone(
                m_boss.isFacingRight() ? bBox.left + bBox.width : bBox.left - bRange,
                bBox.top, bRange, bBox.height
            );

            if (bHitzone.intersects(m_playerModel.getHitbox())) {
                m_playerModel.takeDamage(m_boss.getAttackDamage()); //
                m_boss.m_hasDealtDamage = true;
            }
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
                m_playerModel.setPosition(pPos.x, wall.top); //
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
                m_boss.setVelocity(sf::Vector2f(bVel.x, 0)); //
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
    m_hud.draw(m_window, m_playerModel.getHP(), m_playerModel.getPosition());

    m_window.display();
}

void Game::loadCaveLevel() {
    std::cout << "--- TRANSITION VERS LA GROTTE ---" << std::endl;

    m_currentLevelId = 2;

    // 1. Changer les données du niveau
    m_level.loadLevel(2);     // Charge le tableau de la grotte (dans LevelModel)

    m_levelView.loadTileset("resources/Tileset_Cave.png");

    m_levelView.build(m_level); // Reconstruit l'image (LevelView)

    // 2. Changer le Background (On suppose que tu as ajouté clearLayers dans Background.h)
    // m_background.clearLayers();
    // m_background.addLayer("resources/cave_bg.png"); // Si tu as une image

    // 3. Replacer le joueur au début de la grotte
    m_playerModel.setPosition(100.0f, 300.0f);
    m_playerModel.setVelocity(sf::Vector2f(0, 0));

    // 4. Placer le Boss (Il t'attend plus loin dans la grotte)
    m_boss.setPosition(600.0f, 400.0f);
}
