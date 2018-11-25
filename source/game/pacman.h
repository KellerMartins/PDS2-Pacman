
#ifndef PACMAN_H
#define PACMAN_H

#include <vector>
#include <string>
#include <cmath>

#include "../graphics/animation.h"
#include "enemy.h"
#include "gameEvents.h"
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
	static int x_ToGhost;
	static int y_ToGhost;
	static int dirx_ToGhost;
	static int diry_ToGhost;
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

	bool IsAlive();
	int GetScore();
	unsigned GetLifes();

	virtual void OnUpdate();
	virtual void OnRestart();

	static int get_x();
	static int get_y();
	static int get_dirx();
	static int get_diry();
	
};



#endif