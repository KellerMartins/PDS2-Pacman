#ifndef BLINKY_H
#define BLINKY_H
#include "enemy.h"

class Blinky : public Enemy{
public:
    Blinky(int x, int y);
    virtual void set_goal(int &goal_x, int &goal_y);
};
#endif