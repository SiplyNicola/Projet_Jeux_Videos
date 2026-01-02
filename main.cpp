#include <SFML/Graphics.hpp>
#include "MenuController.h"
#include "Game.h"

#include <ctime>
#include <cstdlib>

/**
 * Whisper of Steel - Main Entry Point
 * This file manages the high-level lifecycle of the application,
 * handling the transitions between the Main Menu and the Game world.
 */
int main() {
    // Initialize the main render window using the desktop's native resolution in Fullscreen mode.
    sf::RenderWindow m_window(sf::VideoMode(1280, 720), "Whisper of Steel", sf::Style::Default);

    // Limit the framerate to 60 FPS to ensure consistent physics and animation timing across different monitors.
    m_window.setFramerateLimit(60);

    // Instantiate the controller responsible for the Main Menu logic and rendering.
    MenuController m_menuController;

    // Initialise the random generator with current time
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // --- GLOBAL APPLICATION LOOP ---
    // This loop keeps the application alive, allowing the user to cycle between the menu and the game.
    while (m_window.isOpen()) {

        // 1. Launch the Menu.
        // The execution blocks inside this function until the user selects "Start" or closes the window.
        m_menuController.run(m_window);

        // 2. Post-Menu Logic.
        // If we reach this point, the menu loop has finished.
        // We check if the window is still open (ensuring the user didn't click "Exit").
        if (m_window.isOpen()) {

            // Instantiate a FRESH instance of the Game engine for each new play session.
            Game m_game(m_window);

            // Start the main game loop.
            // This function runs until a session-ending event occurs (e.g., player death or quitting).
            m_game.run();
        }

        // 3. Cycle Restart.
        // Once m_game.run() finishes, the 'while' loop restarts from the beginning,
        // effectively returning the player to the Main Menu.
    }

    return 0;
}
