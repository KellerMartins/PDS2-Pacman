#include "GameEvents.h"
#ifndef PACMAN_H
#define PACMAN_H
#include <string>

class Pacman : public GameEvents {
private:
	int pontuacao = 0;
	int vidas;
	int direcao_y;
	int direcao_x;
	int x, y;
public:
	Pacman(int x, int y);
	void calcula_direcao();
	void mover(vector<enemy> enemies);
	int verifica_posicao(vector<enemy> enemies);
	void morrer();
	void OnUpdate();
};



#endif