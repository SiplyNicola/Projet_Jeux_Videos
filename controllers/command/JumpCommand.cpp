#include "JumpCommand.h"
#include "PlayerModel.h"

JumpCommand::JumpCommand(PlayerModel& p) : m_player(p) {}

void JumpCommand::execute() {
    m_player.jump();
}
