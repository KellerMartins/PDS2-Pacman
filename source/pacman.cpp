#include "pacman.h"

#include "utils.h"

std::string walk[] = {"assets/models/pacman/walk/player_walk_0.obj",
				  	  "assets/models/pacman/walk/player_walk_1.obj",
					  "assets/models/pacman/walk/player_walk_2.obj",
					  "assets/models/pacman/walk/player_walk_3.obj",
					  "assets/models/pacman/walk/player_walk_4.obj",
					  "assets/models/pacman/walk/player_walk_5.obj"};

std::string idle[] = {"assets/models/pacman/idle/player_idle_0.obj"};

using namespace std;
#define VALOR_FRUTA 50

Pacman::Pacman(int x, int y){
	this->model.Load3DModel("assets/models/pacman/walk/player_walk_0.obj", BLUE);
	this->model.position = (Vector3){(float)x, 0, (float)y};
	this->x = x;
	this->y = y;
	this->direcao_y = 1;
	this->direcao_x = 0;
	this->vidas = 3;
	this->velocidade = 2.5;
	this->timerAnimacao = 0.0;
}

bool Pacman::verifica_posicao(){
	int ix = roundf(this->x + this->direcao_x*0.5);
	int iy = roundf(this->y + this->direcao_y*0.5);

	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Parede) {
		return true;
	}
	
	/*for (unsigned int i = 0; i < enemies.size(); ++i)
	{	//Nao sei se entendi certo essa parte 
		//int Enemy_x = enemies[i]._X();
		//int Enemy_y = enemies[i]._Y();
		
		int Enemy_x = enemies[i]->get_x();
		int Enemy_y = enemies[i]->get_y();

		if(Enemy_x == this->x || Enemy_y== this->y ){
			if(enemies[i]->get_isScared()){
				enemies[i]->morrer();
				this->pontuacao += 50;
			}else{
				this->morrer();
				return 0;				
			}
		};
	}*/
	
	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Ponto) {
		this->pontuacao++;
		//Altera o valor no mapa para que o ponto desapareça
		Mapa::RemoveElementoMapa(ix,iy);
	}
	
	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Especial){
		/*for (unsigned int i = 0; i < enemies.size(); ++i){
			enemies[i]->getScared();
		}*/
		this->pontuacao+= VALOR_FRUTA;
		//Altera o valor no mapa para que o ponto desapareça
		Mapa::RemoveElementoMapa(ix,iy);
	}
	
	return false;
}

void Pacman::calcula_direcao(){
	RenderManager::DrawDebugCube((Vector3){ceilf(x-1), 0.5, roundf(y)}, (Vector3){1,1,1}, WHITE);
	RenderManager::DrawDebugCube((Vector3){floorf(x+1), 0.5, roundf(y)}, (Vector3){1,1,1}, WHITE);
	RenderManager::DrawDebugCube((Vector3){roundf(x), 0.5, ceilf(y-1)}, (Vector3){1,1,1}, WHITE);
	RenderManager::DrawDebugCube((Vector3){roundf(x), 0.5, floorf(y+1)}, (Vector3){1,1,1}, WHITE);

	if (IsKeyPressed(KEY_LEFT) && Mapa::GetElementoMapa(ceilf(x-1),roundf(y)) != ElementoMapa::Parede)
	{
		this->model.rotationAngle = -90;
		this->y = roundf(y);
		this->direcao_y = 0;
		this->direcao_x = -1;
	}
	else if (IsKeyPressed(KEY_RIGHT) && Mapa::GetElementoMapa(floorf(x+1),roundf(y)) != ElementoMapa::Parede)
	{
		this->model.rotationAngle = 90;
		this->y = roundf(y);
		this->direcao_y = 0;
		this->direcao_x = 1;
	}
	else if (IsKeyPressed(KEY_UP) && Mapa::GetElementoMapa(roundf(x),ceilf(y-1)) != ElementoMapa::Parede)
	{
		this->model.rotationAngle = 180;
		this->x = roundf(x);
		this->direcao_y = -1;
		this->direcao_x = 0;
	}
	else if(IsKeyPressed(KEY_DOWN) && Mapa::GetElementoMapa(roundf(x),floorf(y+1)) != ElementoMapa::Parede)
	{
		this->model.rotationAngle = 0;
		this->x = roundf(x);
		this->direcao_y = 1;
		this->direcao_x = 0;
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
	this->calcula_direcao();

	this->timerAnimacao += GetFrameTime() * 9/*Frames por segundo*/;
	
	
	bool colidiu = this->verifica_posicao();
	if(!colidiu){
		this->x = fModulus(this->x + this->direcao_x*this->velocidade*GetFrameTime(), LARGURA);
		this->y = fModulus(this->y + this->direcao_y*this->velocidade*GetFrameTime(), ALTURA);

		timerAnimacao = (int)timerAnimacao > 5/*Num frames - 1*/ ? 0 : timerAnimacao;
		this->model.Load3DModel(walk[(int)timerAnimacao]);
	}else{
		timerAnimacao = (int)timerAnimacao > 0/*Num frames - 1*/ ? 0 : timerAnimacao;
		this->model.Load3DModel(idle[(int)timerAnimacao]);
	}

	this->model.position = (Vector3){(float)x, 0, (float)y};

	
	
	

	RenderManager::DrawDebugCube((Vector3){(float)x, 0.5, (float)y}, (Vector3){1,1,1}, WHITE);
}
