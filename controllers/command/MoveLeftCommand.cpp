#include "MoveLeftCommand.h"
#include "PlayerModel.h"

MoveLeftCommand::MoveLeftCommand(PlayerModel& p) : m_player(p){}

void MoveLeftCommand::execute() {
    m_player.move(-1.0f);
}
