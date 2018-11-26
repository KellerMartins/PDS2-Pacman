#include "pacman.h"

#include "../utils.h"

using namespace std;
#define VALOR_FRUTA 50

int Pacman::x_ToGhost = 0;
int Pacman::y_ToGhost = 0;
int Pacman::dirx_ToGhost = 0;
int Pacman::diry_ToGhost = 0;

Pacman::Pacman(int x, int y) : modelo("assets/models/pacman/idle/player_idle_0.obj", {255,211,0,255}),
							   andando("assets/models/pacman/walk/player_walk", 6, true),
							   parado("assets/models/pacman/idle/player_idle", 1, true),
							   morrendo("assets/models/pacman/die/player_die", 18, false)
{
	this->andando.Preload();
	this->parado.Preload();
	this->morrendo.Preload();
	this->modelo.position = (Vector3){(float)x, 0, (float)y};
	this->x = x;
	this->y = y;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->vivo = true;
	this->vidas = 3;
	this->velocidade = 2.5;
	this->timerAnimacao = 0.0;
}

bool Pacman::verifica_posicao(){
	int ix = roundf(this->x);
	int iy = roundf(this->y);

	std::vector<Enemy*> &enemies = Enemy::get_enemies();
	
	for(unsigned int i = 0; i < enemies.size(); ++i)
	{
		float enemy_x = enemies[i]->get_x();
		float enemy_y = enemies[i]->get_y();

		if(abs(enemy_x - x)<0.5 && abs(enemy_y - y)<0.5){
			if(enemies[i]->isScared()){
				enemies[i]->morrer();
				this->pontuacao += 50;
			}else if(enemies[i]->get_alive()){
				this->morrer();
				return 0;				
			}
		}
	}
	
	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Ponto) {
		this->pontuacao++;
		//Altera o valor no mapa para que o ponto desapareça
		Mapa::RemoveElementoMapa(ix,iy);
	}
	
	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Especial){
		for (unsigned int i = 0; i < enemies.size(); ++i){
			enemies[i]->getScared();
		}
		this->pontuacao+= VALOR_FRUTA;
		//Altera o valor no mapa para que o ponto desapareça
		Mapa::RemoveElementoMapa(ix,iy);
	}

	int nextx = roundf(this->x + this->direcao_x*0.5);
	int nexty = roundf(this->y + this->direcao_y*0.5);
	if(Mapa::GetElementoMapa(nextx,nexty) == ElementoMapa::Parede) {
		return true;
	}else{
		return false;
	}
	
	
}

void Pacman::calcula_direcao(){
	//RenderManager::DrawDebugCube((Vector3){ceilf(x-1), 0.5, roundf(y)}, (Vector3){1,1,1}, WHITE);
	//RenderManager::DrawDebugCube((Vector3){floorf(x+1), 0.5, roundf(y)}, (Vector3){1,1,1}, WHITE);
	//RenderManager::DrawDebugCube((Vector3){roundf(x), 0.5, ceilf(y-1)}, (Vector3){1,1,1}, WHITE);
	//RenderManager::DrawDebugCube((Vector3){roundf(x), 0.5, floorf(y+1)}, (Vector3){1,1,1}, WHITE);

	if (IsKeyPressed(KEY_LEFT) && Mapa::GetElementoMapa(ceilf(x-1),roundf(y)) != ElementoMapa::Parede)
	{
		this->modelo.rotationAngle = -90;
		this->y = roundf(y);
		this->direcao_y = 0;
		this->direcao_x = -1;
	}
	else if (IsKeyPressed(KEY_RIGHT) && Mapa::GetElementoMapa(floorf(x+1),roundf(y)) != ElementoMapa::Parede)
	{
		this->modelo.rotationAngle = 90;
		this->y = roundf(y);
		this->direcao_y = 0;
		this->direcao_x = 1;
	}
	else if (IsKeyPressed(KEY_UP) && Mapa::GetElementoMapa(roundf(x),ceilf(y-1)) != ElementoMapa::Parede)
	{
		this->modelo.rotationAngle = 180;
		this->x = roundf(x);
		this->direcao_y = -1;
		this->direcao_x = 0;
	}
	else if(IsKeyPressed(KEY_DOWN) && Mapa::GetElementoMapa(roundf(x),floorf(y+1)) != ElementoMapa::Parede)
	{
		this->modelo.rotationAngle = 0;
		this->x = roundf(x);
		this->direcao_y = 1;
		this->direcao_x = 0;
	}
}

void Pacman::morrer(){
	this->timerAnimacao = 0;
	this->vivo = false;
	if(vidas>0)
		this->vidas--;
}

void Pacman::Reset(){
	this->vivo = true;
	this->vidas = 3;
	this->velocidade = 2.5;
	this->timerAnimacao = 0.0;
}

void Pacman::OnUpdate(){
	
	this->timerAnimacao += GetFrameTime() * 9 /*Frames por segundo*/;
	
	if(this->vivo){
		this->calcula_direcao();
		bool colidiu = this->verifica_posicao();
		if(!colidiu && (this->direcao_x != 0 || this->direcao_y != 0)){
			this->x = fModulus(this->x + this->direcao_x*this->velocidade*GetFrameTime(), LARGURA);
			this->y = fModulus(this->y + this->direcao_y*this->velocidade*GetFrameTime(), ALTURA);

			andando.SetTimer(timerAnimacao);
			this->modelo.Load3DModel(andando.GetCurrentFrame());
		}else{
			parado.SetTimer(timerAnimacao);
			this->modelo.Load3DModel(parado.GetCurrentFrame());
		}
	}else{
		morrendo.SetTimer(timerAnimacao);
		this->modelo.Load3DModel(morrendo.GetCurrentFrame());
	}
	this->modelo.position = (Vector3){(float)x, 0, (float)y};

	RenderManager::CameraFollow(this->modelo.position);

	x_ToGhost = round(this->x);
	y_ToGhost = round(this->y);
	dirx_ToGhost = this->direcao_x;
	diry_ToGhost = this->direcao_y;
}

void Pacman::OnRestart(){
	Vector2 spawn = Mapa::GetPlayerSpawn();
	modelo.position = {spawn.x, 0, spawn.y};
	x = spawn.x;
	y = spawn.y;
	direcao_y = 0;
	direcao_x = 0;
	vivo = true;
	parado.SetTimer(0);
	modelo.Load3DModel(parado.GetCurrentFrame());
}

int Pacman::get_x(){
	return x_ToGhost;
}
int Pacman::get_y(){
	return y_ToGhost;
}
int Pacman::get_dirx(){
	return dirx_ToGhost;
}
int Pacman::get_diry(){
	return diry_ToGhost;
}

bool Pacman::IsAlive(){
	return vivo;
}

unsigned Pacman::GetScore(){
	return pontuacao;
}

unsigned Pacman::GetLifes(){
	return vidas;
}
