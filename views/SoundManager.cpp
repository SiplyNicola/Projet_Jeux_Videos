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
    /*loadSound(m_attackBuffer, "resources/audio/attack.wav");
    loadSound(m_hitBuffer, "resources/audio/hit.wav");*/

    // On associe les lecteurs aux fichiers
    /*m_attackSound.setBuffer(m_attackBuffer);
    m_hitSound.setBuffer(m_hitBuffer);*/

    // Réglage du volume (optionnel)
    m_jumpSound.setVolume(100.0f);
    m_dashSound.setVolume(100.0f);
    //m_attackSound.setVolume(70.0f);
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

/*void SoundManager::playAttack() {
    // Si le son joue déjà, on le stop pour le relancer (pour spammer l'attaque)
    if (m_attackSound.getStatus() == sf::Sound::Playing) {
        m_attackSound.stop();
    }
    m_attackSound.play();
}

void SoundManager::playHit() {
    m_hitSound.play();
}*/
