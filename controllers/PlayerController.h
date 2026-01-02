#ifndef PLAYERCONTROLLER_H_INCLUDED
#define PLAYERCONTROLLER_H_INCLUDED

#include "PlayerModel.h"
#include "../views/SoundManager.h"

class PlayerController {
public:
    void handleInput(PlayerModel& model, SoundManager& soundManager);
};

#endif // PLAYERCONTROLLER_H_INCLUDED
