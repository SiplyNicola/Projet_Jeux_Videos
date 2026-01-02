#ifndef MOVELEFTCOMMAND_H
#define MOVELEFTCOMMAND_H

#include <ICommand.h>
#include <PlayerModel.h>


class MoveLeftCommand : public ICommand
{
    public:
        explicit MoveLeftCommand(PlayerModel& p);
        void execute() override;

    protected:

    private:
        PlayerModel& m_player;
};

#endif // MOVELEFTCOMMAND_H
