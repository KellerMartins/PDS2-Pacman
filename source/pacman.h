
#ifndef PACMAN_H
#define PACMAN_H
#include "gameEvents.h"
#include "enemy.h"
#include "mapa.h"
#include <vector>
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
	void mover(std::vector<Enemy*> enemies);
	int verifica_posicao(std::vector<Enemy*> enemies);
	void morrer();
	virtual void OnUpdate();
};



#endif