#ifndef JUMPCOMMAND_H
#define JUMPCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>
#include "../views/SoundManager.h"


class JumpCommand : public ICommand
{
    public:
        explicit JumpCommand(PlayerModel& p, SoundManager& soundManager);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
        SoundManager& m_soundManager;
};

#endif // JUMPCOMMAND_H
