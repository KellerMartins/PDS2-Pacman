#ifndef GAME_H
#define GAME_H

#include "../graphics/rendering.h"

namespace Game{

    enum State{Intro, Menu, GameStart, GameInProgress, LevelCleared, GameOver};

    void Init(int screenWidth, int screenHeight, bool fullscreen, RenderManager::BlurQuality quality);
    void Quit();

    void SetState(State state);
    void Update();
}

#endif