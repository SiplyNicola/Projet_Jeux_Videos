#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>
#include "../views/SoundManager.h"


class AttackCommand : public ICommand
{
    public:
        explicit AttackCommand(PlayerModel& p, SoundManager& soundManager);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
        SoundManager& m_soundManager;
};

#endif // ATTACKCOMMAND_H
