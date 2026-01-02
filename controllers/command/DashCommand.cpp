#include "DashCommand.h"
#include "PlayerModel.h"

DashCommand::DashCommand(PlayerModel& p, SoundManager& soundManager) : m_player(p), m_soundManager(soundManager) {}

void DashCommand::execute() {
    if(m_player.dash()){
        m_soundManager.playDash();
    }
}
