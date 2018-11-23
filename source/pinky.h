#ifndef PINKY_H
#define PINKY_H
#include "enemy.h"

class Pinky : public Enemy{
public:
    Pinky(int x, int y);
    virtual void set_goal(int &goal_x, int &goal_y);
};
#endif