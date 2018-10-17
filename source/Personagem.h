#ifndef PERSONAGEM_H_
#define PERSONAGEM_H_
#include <string>

class Personagem {
public:
	int direcao_x, direcao_y;
	int x;//posicao
	int y;
	std::string orientacao; //direita, esquerda, cima, baixo

	Personagem();
	virtual void mover() = 0;
	virtual void colisao() = 0;
	virtual void morrer() = 0;//caso for ter animacao
};



#endif