#include "Game.h"
#include <iostream>
#include <cmath>
Game::Game() : m_background(sf::Vector2u(1280, 720)) {
    m_window.create(sf::VideoMode(1920, 1080), "Whisper of Steel - Final");
    m_window.setFramerateLimit(60);

    // Caméra standard 1280x720
    m_camera.setSize(1280, 720);

    // Initialisation
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel();
    m_levelView.build(m_level);

    // Background layers
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

        // Note : Le saut est maintenant géré par m_playerController
        // dans handleInput, donc on ne met rien ici pour 'Z'.
    }

    // Le contrôleur gère les inputs (Z, Q, D, Espace...)
    m_playerController.handleInput(m_playerModel);
}
void Game::update(float dt) {
    // 1. Mise à jour de la Physique (Joueur et Boss)
    m_playerModel.update(dt);
    m_boss.update(dt, m_playerModel.position);

    // 2. Gestion des Collisions (Murs, Sol, Plafond)
    handleCollisions();
    handleBossCollisions();

    // 3. --- SYSTEME DE RESPAWN (En cas de chute) ---
    // On calcule la limite basse du niveau (Hauteur de la grille * Taille des blocs)
    // m_level.getMapData().size() donne le nombre de lignes.
    // 72.0f est la taille finale d'un bloc (24 * 3).
    float mapHeightPixel = m_level.getMapData().size() * 72.0f;

    // Si le joueur est tombé plus bas que le niveau (+ une marge de 200px)
    if (m_playerModel.position.y > mapHeightPixel + 200.0f) {

        // On le téléporte à la case départ (Bas-Gauche)
        // (Ces coordonnées doivent correspondre à celles dans PlayerModel.cpp)
        m_playerModel.position = sf::Vector2f(150.0f, 2000.0f);

        // IMPORTANT : On annule sa vitesse pour qu'il ne continue pas de tomber
        m_playerModel.velocity = sf::Vector2f(0.0f, 0.0f);
        m_playerModel.state = PlayerState::IDLE;
    }

    // 4. Mise à jour des Animations (Vues)
    m_playerView.updateAnimation(m_playerModel, dt);
    m_bossView.update(dt, m_boss);

    // 5. --- GESTION CAMERA (Suivi + Blocage aux bords) ---
    // Calcul des dimensions totales du monde
    float mapWidth = m_level.getMapData()[0].size() * 72.0f;
    // mapHeightPixel est déjà calculé plus haut

    // Taille de la vue (1280x720)
    sf::Vector2f viewSize = m_camera.getSize();
    float halfW = viewSize.x / 2.0f; // 640
    float halfH = viewSize.y / 2.0f; // 360

    // Position idéale de la caméra (centrée sur le joueur)
    float camX = m_playerModel.position.x;
    float camY = m_playerModel.position.y;

    // CLAMPING X : On empêche la caméra de sortir à gauche ou à droite
    if (camX < halfW) camX = halfW;
    if (camX > mapWidth - halfW) camX = mapWidth - halfW;

    // CLAMPING Y : On empêche la caméra de sortir en haut ou en bas
    if (camY < halfH) camY = halfH;
    if (camY > mapHeightPixel - halfH) camY = mapHeightPixel - halfH;

    // Application de la position finale
    m_camera.setCenter(camX, camY);
    m_window.setView(m_camera);

    // 6. Mise à jour du Background (Parallaxe)
    // Le fond doit savoir où est le coin haut-gauche de la caméra
    float camLeft = m_camera.getCenter().x - halfW;
    float camTop  = m_camera.getCenter().y - halfH;

    m_background.update(camLeft, camTop);
}

void Game::handleCollisions() {
    // 1. Récupération des murs proches
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(m_playerModel.position.x, m_playerModel.position.y);
    sf::FloatRect pBox = m_playerModel.getHitbox();

    for (const auto& wall : walls) {
        if (pBox.intersects(wall)) {

            // --- COLLISION SOL ---
            // Si le joueur tombe (velocity.y > 0) ET qu'il est au-dessus du mur
            if (m_playerModel.velocity.y > 0 && pBox.top + pBox.height < wall.top + 30) {

                // 1. On le pose sur le mur (Correction Position)
                m_playerModel.position.y = wall.top;

                // 2. On arrête la chute (Correction Physique)
                m_playerModel.velocity.y = 0;

                // 3. CORRECTION ANIMATION (Le fix est ici !)
                // Si on était en saut ou chute, on atterrit.
                if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {

                    // On vérifie la vitesse horizontale (X).
                    // Si elle est supérieure à 0.5, c'est qu'on court encore !
                    if (std::abs(m_playerModel.velocity.x) > 0.5f) {
                        m_playerModel.state = PlayerState::RUN;
                    } else {
                        // Sinon, on est à l'arrêt
                        m_playerModel.state = PlayerState::IDLE;
                    }
                }
            }
        }
    }
}

void Game::handleBossCollisions() {
    // 1. Récupérer les murs autour du Boss
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(m_boss.getPosition().x, m_boss.getPosition().y);
    sf::FloatRect bossBox = m_boss.getHitbox();

    for (const auto& wall : walls) {
        if (bossBox.intersects(wall)) {

            // --- CORRECTION DU BUG DE CHUTE ---

            // Calcul de combien le boss est rentré dans le sol
            float overlapY = (bossBox.top + bossBox.height) - wall.top;

            // Collision SOL :
            // 1. On vérifie s'il tombe (Velocity Y > 0)
            // 2. On vérifie s'il n'est pas TROP profond (overlapY).
            //    Avant c'était 25 ou 30, on passe à 60 (presque la taille d'un bloc) pour être sûr.
            if (m_boss.getVelocity().y > 0 && overlapY < 70.0f) {

                // STOP ! On arrête la chute
                m_boss.setVelocity(sf::Vector2f(m_boss.getVelocity().x, 0));

                // REPOSITIONNEMENT PRÉCIS
                // La hitbox du boss (dans BossModel) est calculée comme : Y - 50 avec une hauteur de 100.
                // Donc ses pieds sont à Y + 50.
                // Pour que les pieds (Y+50) soient sur le mur (wall.top), il faut :
                // Y = wall.top - 50.

                // Note : Si ton boss semble flotter, augmente le 50. S'il s'enfonce, diminue le.
                m_boss.setPosition(m_boss.getPosition().x, wall.top - 50.0f);
            }
        }
    }
}

void Game::render() {
    m_window.clear(sf::Color(50, 50, 50));
    m_window.setView(m_camera);

    m_window.draw(m_background); // Fond
    m_window.draw(m_levelView);  // Niveau (Triangles transparents)
    m_bossView.draw(m_window);   // Boss
    m_playerView.draw(m_window); // Joueur

    m_window.display();
}
