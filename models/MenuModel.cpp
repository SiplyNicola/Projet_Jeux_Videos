#include "MenuModel.h"

MenuModel::MenuModel() : m_selectedIndex(0), m_maxOptions(3), m_isGameStarted(false), m_currentState(MenuState::MAIN) {}

// Fix: Utilisation correcte du scope MenuModel::
void MenuModel::setSelectedIndex(int index) {
    if (index >= 0 && index < m_maxOptions) {
        m_selectedIndex = index;
    }
}

void MenuModel::nextOption() { m_selectedIndex = (m_selectedIndex + 1) % m_maxOptions; }
void MenuModel::previousOption() { m_selectedIndex = (m_selectedIndex - 1 + m_maxOptions) % m_maxOptions; }
int MenuModel::getSelectedIndex() const { return m_selectedIndex; }
bool MenuModel::isGameStarted() const { return m_isGameStarted; }
void MenuModel::startGame() { m_isGameStarted = true; }
