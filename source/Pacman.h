#include "GameEvents.h"
#include "Enemy.h"
#include "Mapa.h"
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
	void mover(std::vector<Enemy> enemies, Mapa m);
	int verifica_posicao(std::vector<Enemy> enemies, Mapa m);
	void morrer();
	//void OnUpdate();
	//void OnUpdate(std::vector<Enemy> enemies, Mapa m);
};



#endif