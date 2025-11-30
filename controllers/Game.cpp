#include "Game.h"
#include <iostream>

Game::Game()
    : m_background(sf::Vector2u(1280, 720)) // 1. On initialise le background avec la taille de la fenêtre
{
    m_window.create(sf::VideoMode(1280, 720), "Ninja Platformer");
    m_window.setFramerateLimit(60);
    m_camera.setSize(1280, 720);

    // Initialisation
    // m_playerView est déjà initialisée par son constructeur
    m_bossView.init();
    m_levelView.init();
    m_level.loadLevel();
    m_levelView.build(m_level);

    // --- 2. CHARGEMENT DES COUCHES DE FOND (Code de ton collègue adapté) ---
    // Assure-toi que les images sont bien dans resources/background/
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_1.png"); // Ciel (le plus loin)
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_2.png"); // Montagnes
    m_background.addLayer("resources/oak_woods_v1.0/background/background_layer_3.png"); // Arbres (le plus proche)
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
    // 1. Update Physique (Modèle)
    m_playerModel.update(dt);

    // 2. Update Boss (si tu l'as gardé)
    m_boss.update(dt, m_playerModel.position); // CORRECTION : .position (pas getPosition())

    // 3. Collisions Physique (Player vs Level)
    handleCollisions();
    handleBossCollisions();

    // 4. Collisions Gameplay (Player vs Boss)
    // On utilise la nouvelle fonction getHitbox() qu'on vient d'ajouter
    if (m_playerModel.getHitbox().intersects(m_boss.getHitbox())) {
        // AIE ! (Logique de dégâts ici)
    }

    // 5. Update Vues (Animation)
    m_playerView.updateAnimation(m_playerModel, dt);
    m_bossView.update(dt, m_boss); // Si tu utilises le boss

    // 6. Caméra (Centrée sur le joueur)
    m_camera.setCenter(m_playerModel.position); // CORRECTION : .position
    m_window.setView(m_camera);

    // --- AJOUT : MISE À JOUR DU BACKGROUND ---
    // Le background a besoin de savoir où commence l'écran à gauche (Camera X)
    // Calcul : Centre de la caméra - (Largeur écran / 2)
    float cameraX = m_camera.getCenter().x - (m_camera.getSize().x / 2.0f);
    float cameraY = m_camera.getCenter().y - (m_camera.getSize().y / 2.0f); // <--- AJOUT

    // On met à jour le fond pour qu'il suive
    m_background.update(cameraX, cameraY);
}

void Game::handleCollisions() {
    // --- NOUVELLE LOGIQUE DE COLLISION (Adaptée au code de ton collègue) ---

    // 1. On récupère les murs proches
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(m_playerModel.position.x, m_playerModel.position.y);
    sf::FloatRect playerBox = m_playerModel.getHitbox();

    for (const auto& wall : walls) {
        if (playerBox.intersects(wall)) {

            // Collision SOL (On tombe et on touche le haut d'un mur)
            // On vérifie velocity.y > 0 (chute)
            if (m_playerModel.velocity.y > 0 && playerBox.top + playerBox.height < wall.top + 25) {

                // Correction Position : On le pose sur le mur
                m_playerModel.position.y = wall.top;

                // Correction Vitesse : On arrête la chute
                m_playerModel.velocity.y = 0;

                // Correction État : Si on tombait, on passe en IDLE ou RUN
                if (m_playerModel.state == PlayerState::JUMP || m_playerModel.state == PlayerState::FALL) {
                    if (m_playerModel.velocity.x != 0)
                        m_playerModel.state = PlayerState::RUN;
                    else
                        m_playerModel.state = PlayerState::IDLE;
                }
            }
            // Ici, tu pourrais ajouter les collisions latérales (droite/gauche) si besoin
        }
    }
}

// Dans Game.cpp, ajoute cette méthode (déclare-la aussi dans Game.h !)
void Game::handleBossCollisions() {
    // 1. Récupérer les murs autour du Boss
    std::vector<sf::FloatRect> walls = m_level.getNearbyWalls(m_boss.getPosition().x, m_boss.getPosition().y);
    sf::FloatRect bossBox = m_boss.getHitbox();

    for (const auto& wall : walls) {
        if (bossBox.intersects(wall)) {

            // Collision SOL (Le Boss tombe sur un mur)
            // On vérifie qu'il descend (velocity.y > 0) et qu'il est au-dessus
            if (m_boss.getVelocity().y > 0 && bossBox.top + bossBox.height < wall.top + 25) {

                // 1. On le pose sur le mur
                m_boss.setPosition(m_boss.getPosition().x, wall.top - (bossBox.height / 3.0f));

                // 2. On arrête sa chute
                m_boss.setVelocity(sf::Vector2f(m_boss.getVelocity().x, 0));
            }
        }
    }
}

void Game::render() {
    m_window.clear(sf::Color(50, 50, 50)); // Fond gris
    m_window.setView(m_camera);

    // --- AJOUT : DESSIN DU BACKGROUND EN PREMIER ---
    m_window.draw(m_background);
    // -----------------------------------------------

    m_levelView.draw(m_window);
    m_bossView.draw(m_window);
    m_playerView.draw(m_window);

    m_window.display();
}
