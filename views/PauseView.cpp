#include "PauseView.h"
#include <cmath>

PauseView::PauseView() {}
PauseView::~PauseView() {}

// Copy and Assignment (SFML Resources like Font/Texture aren't easily copyable,
// so we re-link them to ensure stability)
PauseView::PauseView(const PauseView& m_other) { *this = m_other; }
PauseView& PauseView::operator=(const PauseView& m_other) { return *this; }

bool PauseView::init(sf::RenderWindow& m_window) {
    if (!m_font.loadFromFile("resources/fonts/PressStart2P-regular.ttf")) return false;

    m_dimmer.setSize(sf::Vector2f(m_window.getSize()));
    m_dimmer.setFillColor(sf::Color(0, 0, 0, 180)); // Semi-transparent black

    // MAIN PAUSE OPTIONS
    std::vector<std::string> m_labels = { "RESUME", "SETTINGS", "MAIN MENU", "EXIT" };
    for (int i = 0; i < (int)m_labels.size(); ++i) {
        sf::Text m_text;
        m_text.setFont(m_font);
        m_text.setString(m_labels[i]);
        m_text.setCharacterSize(30);
        m_text.setFillColor(sf::Color::Yellow);
        m_text.setOutlineColor(sf::Color::Red);
        m_text.setOutlineThickness(2.0f);

        sf::FloatRect m_rect = m_text.getLocalBounds();
        m_text.setOrigin(m_rect.left + m_rect.width / 2.0f, m_rect.top + m_rect.height / 2.0f);
        m_text.setPosition(m_window.getSize().x / 2.0f, m_window.getSize().y * 0.40f + (i * 80.0f));
        m_optionsTexts.push_back(m_text);
    }

    // SETTINGS / CONTROLS
    std::vector<std::string> m_ctrls = { "CONTROLS", "Right: D", "Left : Q", "Jump : Z", "Dash : J", "Attack: K", "", "Press ESC to back" };
    for (int i = 0; i < (int)m_ctrls.size(); ++i) {
        sf::Text m_text;
        m_text.setFont(m_font);
        m_text.setString(m_ctrls[i]);
        m_text.setCharacterSize(i == 0 ? 35 : 18);
        m_text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::Yellow);

        sf::FloatRect m_rect = m_text.getLocalBounds();
        m_text.setOrigin(m_rect.left + m_rect.width / 2.0f, m_rect.top + m_rect.height / 2.0f);
        m_text.setPosition(m_window.getSize().x / 2.0f, 180.0f + (i * 55.0f));
        m_settingsTexts.push_back(m_text);
    }
    return true;
}

void PauseView::draw(sf::RenderWindow& m_window, const PauseModel& m_model) {
    m_window.draw(m_dimmer);

    if (m_model.getState() == PauseState::MAIN) {
        float m_pulse = 1.0f + std::sin(m_animClock.getElapsedTime().asSeconds() * 6.0f) * 0.05f;
        for (int i = 0; i < (int)m_optionsTexts.size(); ++i) {
            if (i == m_model.getSelectedIndex()) {
                m_optionsTexts[i].setScale(m_pulse * 1.1f, m_pulse * 1.1f);
                m_optionsTexts[i].setFillColor(sf::Color(255, 255, 200));
            } else {
                m_optionsTexts[i].setScale(1.0f, 1.0f);
                m_optionsTexts[i].setFillColor(sf::Color::Yellow);
            }
            m_window.draw(m_optionsTexts[i]);
        }
    } else {
        for (const auto& m_t : m_settingsTexts) m_window.draw(m_t);
    }
}
