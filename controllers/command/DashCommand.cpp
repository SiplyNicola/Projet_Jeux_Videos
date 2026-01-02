#include "DashCommand.h"
#include "PlayerModel.h"

DashCommand::DashCommand(PlayerModel& p) : m_player(p) {}

void DashCommand::execute() {
    m_player.dash();
}
