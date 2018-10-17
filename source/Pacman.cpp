#include "Pacman.h"
#include <string>

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_BOLA 3
#define ITEM_FRUTA 4

Pacman::Pacman(){
	this->x = 0;
	this->y = 0;
	this->orientacao = "teste";
}

int Pacman::verifica_posicao(){
	if(m.GetElementoMapa(this->x,this->y) == MAPA_INVALIDO) return MAPA_INVALIDO;
	if(/*colidiu com inimido*/) return POSICAO_INIMIGO;
	if(m.GetElementoMapa(this->x,this->y) == ITEM_BOLA) return ITEM_BOLA;
	if(m.GetElementoMapa(this->x,this->y) == ITEM_FRUTA) return ITEM_FRUTA;
	if(m.GetElementoMapa(this->x,this->y) == MAPA_VALIDO)	return MAPA_VALIDO;
}

void Pacman::calcula_direcao(){
	if (IsKeyPressed('KEY_LEFT'))
	{
		this->direcao_y = 0;
		this->direcao_x = 1;
	}
	else if (IsKeyPressed('KEY_RIGHT'))
	{
		this->direcao_y = 0;
		this->direcao_x = -1;
	}
	else if (IsKeyPressed('KEY_UP'))
	{
		this->direcao_y = -1;
		this->direcao_x = 0;
	}
	else if(IsKeyPressed('KEY_DOWN')){
		this->direcao_y = 1;
		this->direcao_x = 0;
	}
	else{
		this->direcao_y = 0;
		this->direcao_x = 0;
	}
}

void Pacman::mover(){
	this->calcula_direcao();
	
	//Altera a direção efetivamente
	this->x += this->direcao_x;
	this->y += this->direcao_y;
	int event = this->verifica_posicao();
	
	switch event:
	
	case MAPA_INVALIDO: //Retorna pra posição anterior, pois não pode alcançar a posição desejada
	this->x -= this->direcao_x;
	this->y -= this->direcao_y;
	break;

	case POSICAO_INIMIGO:
	this->morrer();
	break;

	case ITEM_FRUTA:
	this->morrer();
	break;

	case ITEM_BOLINHA:
	this->morrer();
	break;
	
	case 3:
	this->pontuacao++;
	break;
	
	default:
	break;
}
