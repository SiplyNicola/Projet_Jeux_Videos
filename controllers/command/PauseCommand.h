#ifndef PAUSECOMMAND_H
#define PAUSECOMMAND_H

#include "ICommand.h"
#include <functional>

class PauseCommand : public ICommand
{
    public:
        // On prend une fonction vide en paramètre (le callback)
        explicit PauseCommand(std::function<void()> onPauseToggle);
        void execute() override;

    private:
        std::function<void()> m_onPause;
};

#endif
