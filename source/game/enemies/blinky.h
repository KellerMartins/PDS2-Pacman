#ifndef BLINKY_H
#define BLINKY_H
#include "../enemy.h"

class Blinky : public Enemy{
public:
    Blinky(int x, int y);
    virtual void SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY);
};
#endif