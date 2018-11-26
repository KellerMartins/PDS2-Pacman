#ifndef PINKY_H
#define PINKY_H
#include "../enemy.h"

class Pinky : public Enemy{
public:
    Pinky(int x, int y);
    virtual void SetGoal(int &goalX, int &goal_y, int pacX, int pac_y,int dirX, int dirY);
};
#endif