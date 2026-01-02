#include "MenuModel.h"

/**
 * MenuModel Constructor
 * Initializes the menu state with the first option selected.
 *
 */
MenuModel::MenuModel()
    : m_selectedIndex(0),      // Start with the first menu item highlighted
      m_maxOptions(3),         // Define the total number of available menu options
      m_isGameStarted(false),  // Initial state: the game has not yet transitioned to the play state
      m_currentState(MenuState::MAIN) // Set the initial view to the Main Menu
{}

/**
 * Manually sets the current selected index.
 * Includes a safety check to ensure the index remains within valid bounds.
 * @param index The new index to select.
 */
void MenuModel::setSelectedIndex(int index) {
    // Boundary check: ensure the index is between 0 and the maximum number of options
    if (index >= 0 && index < m_maxOptions) {
        m_selectedIndex = index;
    }
}

/**
 * Moves the selection to the next available option.
 * Uses the modulo operator to cycle back to the first option after the last one.
 */
void MenuModel::nextOption() {
    m_selectedIndex = (m_selectedIndex + 1) % m_maxOptions;
}

/**
 * Moves the selection to the previous available option.
 * Uses a mathematical wrap-around logic to cycle to the last option if moving back from the first.
 */
void MenuModel::previousOption() {
    m_selectedIndex = (m_selectedIndex - 1 + m_maxOptions) % m_maxOptions;
}

/**
 * @return The index of the currently highlighted menu option.
 */
int MenuModel::getSelectedIndex() const {
    return m_selectedIndex;
}

/**
 * Checks if the game has been triggered to start.
 * Used by the Game class to transition from Menu to Level 1.
 */
bool MenuModel::isGameStarted() const {
    return m_isGameStarted;
}

/**
 * Sets the game-started flag to true.
 * This effectively signals the main game loop to begin the level initialization.
 */
void MenuModel::startGame() {
    m_isGameStarted = true;
}
