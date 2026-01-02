#include "SoundManager.h"
#include <iostream>
#include <cstdlib>

SoundManager::SoundManager() {
    // Constructeur vide
}

void SoundManager::init() {
    // Charge tes fichiers ici (Assure-toi d'avoir les fichiers .wav ou .ogg)
    loadSound(m_jumpBuffer1, "resources/sound/Jump/jump_1.wav");
    loadSound(m_jumpBuffer2, "resources/sound/Jump/jump_2.wav");
    loadSound(m_jumpBuffer3, "resources/sound/Jump/jump_3.wav");

    loadSound(m_dashBuffer1, "resources/sound/Dash/dash_1.wav");
    loadSound(m_dashBuffer2, "resources/sound/Dash/dash_2.wav");
    loadSound(m_dashBuffer3, "resources/sound/Dash/dash_3.wav");
    loadSound(m_dashBuffer4, "resources/sound/Dash/dash_4.wav");

    loadSound(m_slashBuffer, "resources/sound/Slash/slash.wav");

    loadSound(m_atkBuffer1, "resources/sound/Attack/attack_1.wav");
    loadSound(m_atkBuffer2, "resources/sound/Attack/attack_2.wav");
    loadSound(m_atkBuffer3, "resources/sound/Attack/attack_3.wav");

    // On associe les lecteurs aux fichiers
    m_weaponSound.setBuffer(m_slashBuffer);

    // Réglage du volume (optionnel)
    m_jumpSound.setVolume(100.0f);
    m_dashSound.setVolume(100.0f);
    m_weaponSound.setVolume(20.0f);
    m_atkSound.setVolume(200.0f);
}

void SoundManager::playMenuMusic() {
    // Si la musique joue déjà, on l'arrête proprement
    if (m_music.getStatus() == sf::Music::Playing) {
        m_music.stop();
    }

    // CHARGEMENT ET LECTURE (Adapte le chemin du fichier !)
    if (m_music.openFromFile("resources/sound/Musics/MenuMusic.wav")) {
        m_music.setLoop(true);   // La musique tourne en boucle
        m_music.setVolume(50.f); // Volume modéré pour la musique (0 à 100)
        m_music.play();
    }
}

void SoundManager::playLevel1Music() {
    if (m_music.getStatus() == sf::Music::Playing) m_music.stop();

    // Musique de la Forêt
    if (m_music.openFromFile("resources/sound/Musics/ForestMusic.wav")) {
        m_music.setLoop(true);
        m_music.setVolume(40.f);
        m_music.play();
    }
}

void SoundManager::playCaveMusic() {
    if (m_music.getStatus() == sf::Music::Playing) m_music.stop();

    // Musique de la Grotte
    if (m_music.openFromFile("resources/sound/Musics/BossMusic.wav")) {
        m_music.setLoop(true);
        m_music.setVolume(40.f);
        m_music.play();
    }
}

void SoundManager::stopMusic() {
    m_music.stop();
}

void SoundManager::loadSound(sf::SoundBuffer& buffer, const std::string& path) {
    if (!buffer.loadFromFile(path)) {
        std::cerr << "ERREUR AUDIO : Impossible de charger " << path << std::endl;
        // SFML affichera l'erreur détaillée dans la console
    }
}

void SoundManager::playJump() {
    // On ajoute une petite variation de pitch (aigu/grave) pour rendre le son plus vivant
    m_jumpSound.setPitch(0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f)));

    // 1. On tire un nombre au hasard : 0, 1 ou 2
    int randomPick = std::rand() % 3;

    // 2. On assigne le buffer correspondant
    if (randomPick == 0) {
        m_jumpSound.setBuffer(m_jumpBuffer1);
    }
    else if (randomPick == 1) {
        m_jumpSound.setBuffer(m_jumpBuffer2);
    }
    else {
        m_jumpSound.setBuffer(m_jumpBuffer3);
    }

    m_jumpSound.play();
}

void SoundManager::playDash() {
    // On ajoute une petite variation de pitch (aigu/grave) pour rendre le son plus vivant
    m_dashSound.setPitch(0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f)));

    // 1. On tire un nombre au hasard : 0, 1, 2 ou 3
    int randomPick = std::rand() % 4;

    // 2. On assigne le buffer correspondant
    if (randomPick == 0) {
        m_dashSound.setBuffer(m_dashBuffer1);
    }
    else if (randomPick == 1) {
        m_dashSound.setBuffer(m_dashBuffer2);
    }
    else if (randomPick == 2) {
        m_dashSound.setBuffer(m_dashBuffer3);
    }
    else {
        m_dashSound.setBuffer(m_dashBuffer4);
    }

    m_dashSound.play();
}

void SoundManager::playAttack() {
    // --- PARTIE 1 : LE SLASH (Toujours le même) ---
    // Petit bonus : on varie légèrement le pitch (aigu/grave) pour que le slash
    // ne sonne pas robotique à chaque coup.
    float randomPitch = 0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f));
    m_weaponSound.setPitch(randomPitch);
    m_weaponSound.play();

    // --- PARTIE 2 : LA VOIX (Aléatoire) ---
    int randomPick = std::rand() % 3; // 0, 1 ou 2

    if (randomPick == 0) m_atkSound.setBuffer(m_atkBuffer1);
    else if (randomPick == 1) m_atkSound.setBuffer(m_atkBuffer2);
    else m_atkSound.setBuffer(m_atkBuffer3);

    m_atkSound.play();
}
