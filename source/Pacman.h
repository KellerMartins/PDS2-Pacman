#include "Personagem.h"
#ifndef PACMAN_H
#define PACMAN_H
#include <string>

class Pacman : public Personagem {
public:
	int pontuacao = 0;
	Pacman();
	void calcula_direcao();
	void mover();
	void colisao();
	void morrer();
};



#endif