#include "AttackCommand.h"
#include "PlayerModel.h"

AttackCommand::AttackCommand(PlayerModel& p, SoundManager& soundManager) : m_player(p), m_soundManager(soundManager){}

void AttackCommand::execute() {
    if(m_player.attack()){
        m_soundManager.playAttack();
    }
}
