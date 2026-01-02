#ifndef MOVERIGHTCOMMAND_H
#define MOVERIGHTCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>


class MoveRightCommand : public ICommand
{
    public:
        explicit MoveRightCommand(PlayerModel& p);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
};

#endif // MOVERIGHTCOMMAND_H
