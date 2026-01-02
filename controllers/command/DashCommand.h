#ifndef DASHCOMMAND_H
#define DASHCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>


class DashCommand : public ICommand
{
    public:
        explicit DashCommand(PlayerModel& p);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
};

#endif // DASHCOMMAND_H
