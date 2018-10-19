#include "Personagem.h"
#ifndef PACMAN_H
#define PACMAN_H
#include <string>

class Pacman : public Personagem {
private:
	int pontuacao = 0;
	int vidas;
	int direcao_y;
	int direcao_x;
public:
	Pacman(int x, int y);
	void calcula_direcao();
	void mover(vector<enemy> enemies);
	int verifica_posicao(vector<enemy> enemies);
	void colisao();
	void morrer();
	void OnUpdate();
};



#endif