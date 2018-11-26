
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
	Object3D _modelo;
	bool _vivo;
	unsigned int _vidas;
	unsigned int _pontuacao = 0;
	int _direcaoY;
	int _direcaoX;
	float _x, _y;
	static int _xToGhost;
	static int _yToGhost;
	static int _dirxToGhost;
	static int _diryToGhost;
	float _velocidade;
	float _timerAnimacao;
	Animation _andando;
	Animation _parado;
	Animation _morrendo;
public:
	Pacman(int x, int y);
	void CalculaDirecao();
	bool VerificaPosicao();
	void Morrer();
	void Resetar();

	bool IsAlive();
	unsigned GetPontuacao();
	unsigned GetVidas();

	virtual void OnUpdate();
	virtual void OnRestart();

	static int GetX();
	static int GetY();
	static int GetDirX();
	static int GetDirY();
	
};



#endif