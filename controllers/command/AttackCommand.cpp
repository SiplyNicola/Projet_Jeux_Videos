#include "AttackCommand.h"
#include "PlayerModel.h"

AttackCommand::AttackCommand(PlayerModel& p) : m_player(p){}

void AttackCommand::execute() {
    m_player.attack();
}
