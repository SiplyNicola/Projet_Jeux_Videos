#include "PauseCommand.h"

PauseCommand::PauseCommand(std::function<void()> onPauseToggle)
    : m_onPause(onPauseToggle) {}

void PauseCommand::execute()
{
    if (m_onPause) {
        m_onPause();
    }
}
