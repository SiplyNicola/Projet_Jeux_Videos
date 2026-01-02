#ifndef JUMPCOMMAND_H
#define JUMPCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>


class JumpCommand : public ICommand
{
    public:
        explicit JumpCommand(PlayerModel& p);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
};

#endif // JUMPCOMMAND_H
