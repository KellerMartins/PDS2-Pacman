#ifndef PERSONAGEM_H_
#define PERSONAGEM_H_
#include <string>

class Personagem {
public:

	float x;//posicao
	float y;
	std::string orientacao; //direita, esquerda, cima, baixo

	Personagem();
	void mover();
	void colisao();
	void morrer();//caso for ter animacao
};



#endif