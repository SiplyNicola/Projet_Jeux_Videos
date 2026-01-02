#include "MoveRightCommand.h"
#include "PlayerModel.h"

MoveRightCommand::MoveRightCommand(PlayerModel& p) : m_player(p){}

void MoveRightCommand::execute() {
    m_player.move(1.0f);
}
