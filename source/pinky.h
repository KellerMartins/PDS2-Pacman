#ifndef PINKY_H
#define PINKY_H
#include "enemy.h"

class Pinky : public Enemy{
public:
    Pinky(int x, int y);
	//virtual void calcula_direcao() = 0;
};
#endif