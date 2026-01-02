#ifndef DASHCOMMAND_H
#define DASHCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>
#include "../views/SoundManager.h"


class DashCommand : public ICommand
{
    public:
        explicit DashCommand(PlayerModel& p, SoundManager& soundManager);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
        SoundManager& m_soundManager;
};

#endif // DASHCOMMAND_H
