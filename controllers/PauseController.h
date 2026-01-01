#ifndef PAUSECONTROLLER_H
#define PAUSECONTROLLER_H

#include <SFML/Graphics.hpp>
#include "PauseModel.h"
#include "PauseView.h"

class PauseController {
public:
    // --- COPLIEN'S RULE ---
    PauseController();                                          // Default Constructor
    PauseController(const PauseController& m_other);            // Copy Constructor
    PauseController& operator=(const PauseController& m_other); // Assignment Operator
    ~PauseController();                                         // Destructor

    // Main input handling for the pause state
    void handleInput(sf::RenderWindow& m_window, sf::Event& m_event, PauseModel& m_model, const PauseView& m_view);
};

#endif
