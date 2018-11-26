#ifndef CLYDE_H
#define CLYDE_H
#include "../enemy.h"

class Clyde : public Enemy{
public:
    Clyde(int x, int y);
    virtual void SetGoal(int &goalX, int &goalY, int pacX, int pacY,int dirX, int dirY);
};
#endif