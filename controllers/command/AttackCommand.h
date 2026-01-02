#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>


class AttackCommand : public ICommand
{
    public:
        explicit AttackCommand(PlayerModel& p);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
};

#endif // ATTACKCOMMAND_H
