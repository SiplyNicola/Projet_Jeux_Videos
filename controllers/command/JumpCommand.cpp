#include "JumpCommand.h"
#include "PlayerModel.h"

JumpCommand::JumpCommand(PlayerModel& p, SoundManager& soundManager) : m_player(p), m_soundManager(soundManager) {}

void JumpCommand::execute() {
    if(m_player.jump()){
        m_soundManager.playJump();
    }
}
