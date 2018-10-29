#include "GameEvents.h"
#ifndef ENEMY_H
#define ENEMY_H
#include <string>

class Enemy : public GameEvents {
private:
	int isScared = 0;
	int direcao_y;
	int direcao_x;
	int x, y;
public:
	Enemy(int x, int y);
	virtual void calcula_direcao() = 0;
	void mover();
	int verifica_posicao();
	void morrer();
	void OnUpdate();
	void getScared();
	int isScared();
};



#endif