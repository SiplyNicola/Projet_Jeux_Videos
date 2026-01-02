#include "PauseModel.h"

/**
 * PauseModel Constructor
 * Initializes the pause menu state with 4 selectable options.
 * This model manages the state of the game when it is suspended via the 'M' key.
 */
PauseModel::PauseModel()
    : m_selectedIndex(0),           // Start selection at the first item
      m_maxOptions(4),              // Resume, Options, Levels, Main Menu
      m_currentState(PauseState::MAIN),
      m_resumeFlag(false),          // Flag to signal the game loop to continue
      m_exitToMainFlag(false)       // Flag to signal exit to the main menu
{}

/**
 * Copy Constructor
 * Delegates the copy operation to the assignment operator.
 * @param m_other The PauseModel instance to copy from.
 */
PauseModel::PauseModel(const PauseModel& m_other) {
    *this = m_other;
}

/**
 * Assignment Operator
 * Safely copies state from another PauseModel instance while checking for self-assignment.
 * @param m_other The source PauseModel instance.
 * @return A reference to the updated current instance.
 */
PauseModel& PauseModel::operator=(const PauseModel& m_other) {
    if (this != &m_other) {
        m_selectedIndex = m_other.m_selectedIndex;
        m_maxOptions = m_other.m_maxOptions;
        m_currentState = m_other.m_currentState;
        m_resumeFlag = m_other.m_resumeFlag;
        m_exitToMainFlag = m_other.m_exitToMainFlag;
    }
    return *this;
}

/**
 * Destructor
 * Cleans up resources; currently empty as the model uses stack-allocated primitives.
 */
PauseModel::~PauseModel() {}

/**
 * Resets the pause menu state.
 * This is typically called by the Game class when the pause menu is toggled.
 */
void PauseModel::reset() {
    m_selectedIndex = 0;              // Reset selection to the top
    m_currentState = PauseState::MAIN; // Return to the main pause page
    m_resumeFlag = false;             // Reset control flags
    m_exitToMainFlag = false;
}

/**
 * Moves the menu selection to the next option.
 * Uses modulo arithmetic to wrap the selection back to index 0 after the last option.
 */
void PauseModel::nextOption() {
    m_selectedIndex = (m_selectedIndex + 1) % m_maxOptions;
}

/**
 * Moves the menu selection to the previous option.
 * Uses mathematical wrap-around logic to move from the first option to the last.
 */
void PauseModel::previousOption() {
    m_selectedIndex = (m_selectedIndex - 1 + m_maxOptions) % m_maxOptions;
}

/**
 * Manually sets the selected index.
 * @param m_index The specific menu index to highlight.
 */
void PauseModel::setSelectedIndex(int m_index) {
    m_selectedIndex = m_index;
}

/**
 * @return The index of the currently highlighted option.
 */
int PauseModel::getSelectedIndex() const {
    return m_selectedIndex;
}
