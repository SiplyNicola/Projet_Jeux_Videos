#include "PauseModel.h"

PauseModel::PauseModel()
    : m_selectedIndex(0), m_maxOptions(4), m_currentState(PauseState::MAIN),
      m_resumeFlag(false), m_exitToMainFlag(false) {}

PauseModel::PauseModel(const PauseModel& m_other) {
    *this = m_other;
}

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

PauseModel::~PauseModel() {}

void PauseModel::reset() {
    m_selectedIndex = 0;
    m_currentState = PauseState::MAIN;
    m_resumeFlag = false;
    m_exitToMainFlag = false;
}

void PauseModel::nextOption() { m_selectedIndex = (m_selectedIndex + 1) % m_maxOptions; }
void PauseModel::previousOption() { m_selectedIndex = (m_selectedIndex - 1 + m_maxOptions) % m_maxOptions; }
void PauseModel::setSelectedIndex(int m_index) { m_selectedIndex = m_index; }
int PauseModel::getSelectedIndex() const { return m_selectedIndex; }
