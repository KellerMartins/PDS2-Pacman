
#ifndef PACMAN_H
#define PACMAN_H

#include <time.h>
#include <vector>
#include <string>
#include <cmath>

#include "graphics/animation.h"
#include "gameEvents.h"
#include "enemy.h"
#include "mapa.h"

class Pacman : public GameEvents {
private:
	Object3D modelo;
	bool vivo;
	int vidas;
	int pontuacao = 0;
	int direcao_y;
	int direcao_x;
	float x, y;
	float velocidade;
	float timerAnimacao;
	Animation andando;
	Animation parado;
	Animation morrendo;
public:
	Pacman(int x, int y);
	void calcula_direcao();
	bool verifica_posicao();
	void morrer();
	virtual void OnUpdate();
	virtual void OnRestart();
};



#endif