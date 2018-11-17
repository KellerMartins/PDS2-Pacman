#ifndef BLINKY_H
#define BLINKY_H
#include "enemy.h"

class Blinky : public Enemy{
public:
    Blinky(int x, int y);
	//virtual void calcula_direcao() = 0;
};
#endif