#include "Pacman.h"
#include <string>

Pacman::Pacman(){
	this->x = 0;
	this->y = 0;
	this->orientacao = "teste";
}


Pacman::calcula_direcao(){
	if (IsKeyPressed('LEFTARROW'))
	{
		this->direcao_y = 0;
		this->direcao_x = 1;
	}
	else if (IsKeyPressed('RIGHTARROW'))
	{
		this->direcao_y = 0;
		this->direcao_x = -1;
	}
	else if (IsKeyPressed('UPARROW'))
	{
		this->direcao_y = -1;
		this->direcao_x = 0;
	}
	else if(IsKeyPressed('EXDOWNARROW')){
		this->direcao_y = 1;
		this->direcao_x = 0;
	}
	else{
		this->direcao_y = 0;
		this->direcao_x = 0;
	}
}

Pacman::mover(){
	this->calcula_direcao();
	this->x += this->direcao_x;
	this->y += this->direcao_y;
}
