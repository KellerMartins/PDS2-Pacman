
#ifndef PACMAN_H
#define PACMAN_H
#include "gameEvents.h"
#include "enemy.h"
#include "mapa.h"
#include <time.h>
#include <vector>
#include <string>
#include <cmath>

class Pacman : public GameEvents {
private:
	Object3D model;
	int pontuacao = 0;
	int vidas;
	int direcao_y;
	int direcao_x;
	float x, y;
	float velocidade;
	float timerAnimacao;
public:
	Pacman(int x, int y);
	void calcula_direcao();
	bool verifica_posicao();
	void morrer();
	virtual void OnUpdate();
};



#endif