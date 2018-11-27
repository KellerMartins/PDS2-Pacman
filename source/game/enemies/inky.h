#ifndef INKY_H
#define INKY_H
#include "../enemy.h"

class Inky : public Enemy{
public:
    Inky(int x, int y);
    virtual void SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY);
};
#endif