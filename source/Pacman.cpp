#include "Pacman.h"
#include <vector>
#include <string>

using namespace std;

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

Pacman::Pacman(int x, int y){
	this->x = x;
	this->y = y;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->vidas = 3;
}

int Pacman::verifica_posicao(vector<enemy> enemies){
	if(m.GetElementoMapa(this->x,this->y) == MAPA_INVALIDO) {
		return MAPA_INVALIDO;
	}

	for (int i = 0; i < enemies.size(); ++i)
	{
		int enemy_x = enemies[i]._X();
		int enemy_y = enemies[i]._Y();

		if(enemy_x == this->x || enemy_y== this->y ){
			if(enemies[i].isScared()){
				enemies[i].morrer();
				this->pontuacao += 50;
			}else{
				this->morrer();
				return 0;				
			}
		};
	}
	
	if(m.GetElementoMapa(this->x,this->y) == ITEM_PONTO) {
		this->pontuacao++
		//Altera o valor no mapa para que o ponto desapareça
		//m.setElementoZero(this->x,this->y)
		return 0;
	};
	
	if(m.GetElementoMapa(this->x,this->y) == ITEM_FRUTA){
		for (int i = 0; i < enemies.size(); ++i){
			enemies[i].getScared();
		}
		this->pontuacao+= VALOR_FRUTA;
		//Altera o valor no mapa para que o ponto desapareça
		//m.setElementoZero(this->x,this->y)
		return 0;
	};
	
	return 0;
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

void Pacman::mover(vector<enemy> enemies){
	this->calcula_direcao();
	
	//Altera a direção efetivamente
	this->x += this->direcao_x;
	this->y += this->direcao_y;

	if(this->verifica_posicao(enemies)){
	this->x -= this->direcao_x;
	this->y -= this->direcao_y;
	}	
}

void Pacman::morrer(){
	this->x = 0;
	this->y = 0;
	this->vidas--;

	if(!this->vidas){
		//game over
	}
}

void Pacman::OnUpdate(){
	this->mover();
}