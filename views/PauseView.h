#ifndef PAUSEVIEW_H
#define PAUSEVIEW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "PauseModel.h"

class PauseView {
public:
    PauseView();
    PauseView(const PauseView& m_other);
    PauseView& operator=(const PauseView& m_other);
    ~PauseView();

    bool init(sf::RenderWindow& m_window);
    void draw(sf::RenderWindow& m_window, const PauseModel& m_model);
    const std::vector<sf::Text>& getButtons() const { return m_optionsTexts; }

private:
    sf::Font m_font;
    sf::RectangleShape m_dimmer; // Dark background overlay
    std::vector<sf::Text> m_optionsTexts;
    std::vector<sf::Text> m_settingsTexts;
    sf::Clock m_animClock;
};

#endif
