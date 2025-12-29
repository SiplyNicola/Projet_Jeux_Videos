#include <SFML/Graphics.hpp>
#include "MenuController.h"
#include "Game.h"

int main() {
    sf::RenderWindow m_window(sf::VideoMode::getDesktopMode(), "Whisper of Steel", sf::Style::Fullscreen);
    m_window.setFramerateLimit(60);

    MenuController m_menuController;

    // --- BOUCLE GLOBALE DE L'APPLICATION ---
    while (m_window.isOpen()) {

        // 1. On lance le menu. Le code s'arrête ici tant qu'on n'a pas cliqué sur "Start"
        m_menuController.run(m_window);

        // 2. Si on arrive ici, c'est que le menu est fini.
        // On vérifie si la fenêtre est toujours ouverte (si on n'a pas cliqué sur EXIT)
        if (m_window.isOpen()) {
            Game m_game(m_window); // On crée une NOUVELLE instance de jeu
            m_game.run();          // On lance le jeu. Quand le joueur meurt, cette fonction s'arrête.
        }

        // 3. Une fois m_game.run() terminé, la boucle 'while' repart au début et relance m_menuController.run()!
    }

    return 0;
}
