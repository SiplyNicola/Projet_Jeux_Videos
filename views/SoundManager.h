#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>

class SoundManager {
public:
    SoundManager();
    void init();

    // Fonctions pour jouer les sons
    void playJump();
    void playDash();
    void playAttack();

private:
    // Les buffers (les fichiers chargés)
    sf::SoundBuffer m_jumpBuffer1;
    sf::SoundBuffer m_jumpBuffer2;
    sf::SoundBuffer m_jumpBuffer3;

    sf::SoundBuffer m_dashBuffer1;
    sf::SoundBuffer m_dashBuffer2;
    sf::SoundBuffer m_dashBuffer3;
    sf::SoundBuffer m_dashBuffer4;

    sf::SoundBuffer m_slashBuffer;

    sf::SoundBuffer m_atkBuffer1;
    sf::SoundBuffer m_atkBuffer2;
    sf::SoundBuffer m_atkBuffer3;

    // Les lecteurs
    sf::Sound m_jumpSound;
    sf::Sound m_dashSound;
    sf::Sound m_weaponSound;
    sf::Sound m_atkSound;

    // Petite fonction utilitaire pour charger sans faire planter le jeu
    void loadSound(sf::SoundBuffer& buffer, const std::string& path);
};

#endif
