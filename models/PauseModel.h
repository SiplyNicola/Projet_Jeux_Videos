#ifndef PAUSEMODEL_H
#define PAUSEMODEL_H

// Screen states for the pause menu
enum class PauseState { MAIN, SETTINGS };

class PauseModel {
public:
    // --- COPLIEN'S RULE ---
    PauseModel();                                     // Default Constructor
    PauseModel(const PauseModel& m_other);            // Copy Constructor
    PauseModel& operator=(const PauseModel& m_other); // Assignment Operator
    ~PauseModel();                                    // Destructor

    // Navigation and Logic
    void nextOption();
    void previousOption();
    void setSelectedIndex(int m_index);
    int getSelectedIndex() const;

    PauseState getState() const { return m_currentState; }
    void setState(PauseState m_state) { m_currentState = m_state; }

    // Flags for the Game class
    bool shouldResume() const { return m_resumeFlag; }
    bool shouldExitToMain() const { return m_exitToMainFlag; }
    void setResume(bool m_val) { m_resumeFlag = m_val; }
    void setExitToMain(bool m_val) { m_exitToMainFlag = m_val; }

    void reset(); // Re-initializes state when pause is toggled

private:
    int m_selectedIndex;
    int m_maxOptions;
    PauseState m_currentState;
    bool m_resumeFlag;
    bool m_exitToMainFlag;
};

#endif
