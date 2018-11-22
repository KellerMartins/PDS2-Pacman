#include "enemy.h"
#include "utils.h"
using namespace std;

#define MAPA_VALIDO 0
#define MAPA_INVALIDO 1
#define POSICAO_INIMIGO 2
#define ITEM_PONTO 3
#define ITEM_FRUTA 4
#define VALOR_FRUTA 50

std::vector<Enemy*> Enemy::enemies(0);

Enemy::Enemy(int x, int y, Color color){
	this->x = x;
	this->y = y;
	this->color = color;
	this->direcao_y = 0;
	this->direcao_x = 0;
	this->isScared = 0;
	this->velocidade = 2.5;
	this->timerAnimacao = 0.0;
	this->itr_x = 11;//aleatorio
	this->itr_y = 2;

	//Astar
	for(int i = 0; i< LARGURA; i++)
	{
		for(int j = 0; j< ALTURA; j++)
		{
			grid[i][j].f = 0;
			grid[i][j].g = 0;
			grid[i][j].h = 0;
			grid[i][j].i = i;
			grid[i][j].j = j;
			if(Mapa::GetElementoMapa(i,j) == ElementoMapa::Parede) {
				grid[i][j].obstacle = true;
			}
		}
	}
}

void Enemy::adiciona_inimigo(Enemy* enemy){
	enemies.push_back(enemy);
}
void Enemy::remove_inimigos(){
	enemies.clear();
	
}

std::vector<Enemy*> &Enemy::get_enemies()
{
	return enemies;
}
void Enemy::calcula_direcao(int goal_x, int goal_y){

	if(goal_x > this->x)
	{
		this->direcao_y = 0;
		this->direcao_x = 1;
	}
	else if(goal_x < this->x)
	{
		this->direcao_y = 0;
		this->direcao_x = -1;
	}
	else if(goal_y < this->y)
	{
		this->direcao_y = -1;
		this->direcao_x = 0;
	}
	else if(goal_y > this->y)
	{
		this->direcao_y = 1;
		this->direcao_x = 0;
	}

}
bool Enemy::verifica_posicao(){
	int ix = roundf(this->x + this->direcao_x*0.5);
	int iy = roundf(this->y + this->direcao_y*0.5);

	if(Mapa::GetElementoMapa(ix,iy) == ElementoMapa::Parede) {
		return true;
	}

	return false;
}
void Enemy::morrer(){
	this->x = 0;
	this->y = 0;
	this->isScared = 0;
}

void Enemy::OnUpdate(){

	this->calcula_direcao(this->itr_x,this->itr_y);

	this->timerAnimacao += GetFrameTime() * 9/*Frames por segundo*/;
	
	
	bool colidiu = this->verifica_posicao();
	if(!colidiu){
		this->x = fModulus(this->x + this->direcao_x*this->velocidade*GetFrameTime(), LARGURA);
		this->y = fModulus(this->y + this->direcao_y*this->velocidade*GetFrameTime(), ALTURA);
		
		timerAnimacao = (int)timerAnimacao > 5/*Num frames - 1*/ ? 0 : timerAnimacao;
		//this->model.Load3DModel(walk[(int)timerAnimacao]);
	}else{
		timerAnimacao = (int)timerAnimacao > 0/*Num frames - 1*/ ? 0 : timerAnimacao;
		//this->model.Load3DModel(idle[(int)timerAnimacao]);
	}

	//this->model.position = (Vector3){(float)x, 0, (float)y};

	RenderManager::DrawDebugCube((Vector3){(float)x, 0.5, (float)y}, (Vector3){1,1,1}, this->color);
}

void Enemy::getScared(){
	this->isScared = 1;
}

int Enemy::get_isScared(){
	return this->isScared;
}
Color Enemy::get_color(){
	return this->color;
}
int Enemy::get_x(){
	return this->x;
}
int Enemy::get_y(){
	return this->y;
}

//Astar

void Enemy::reset_grid(){
	for(int i = 0; i< LARGURA; i++)
	{
		for(int j = 0; j< ALTURA; j++)
		{
			grid[i][j].f = 0;
			grid[i][j].g = 0;
			grid[i][j].h = 0;
		}
	}
}

std::list<ElementoBusca*> Enemy::calcula_vizinhos(ElementoBusca* elem){

	std::list<ElementoBusca*> vizinhos;
	int i = elem->i;
	int j = elem->j;
	if(i < LARGURA - 1){
		vizinhos.push_back(&grid[i+1][j]);
	}
	if(i > 0)
	{
		vizinhos.push_back(&grid[i-1][j]);
	}
	if(j < ALTURA - 1){
		vizinhos.push_back(&grid[i][j+1]);
	}
	if(j > 0){
		vizinhos.push_back(&grid[i][j-1]);
	}
	
	return vizinhos;

}

ElementoBusca* get_at(std::list<ElementoBusca*> &l, unsigned int i)
{   
    typename std::list<ElementoBusca*>::iterator it;
    if(l.size() > i)
    {
        auto it = l.begin();
        std::advance(it, i);
        return *it;
    }else{
        throw std::invalid_argument("Posicao invalida!");
    }
}

void Enemy::Astar(int start_x, int start_y, int goal_x, int goal_y){
	this->reset_grid();
	std::list<ElementoBusca*> OpenSet;
	std::list<ElementoBusca*> ClosedSet;

	OpenSet.push_back(&grid[start_x][start_y]);
	ElementoBusca* current;
	ElementoBusca* vizinho;
	std::list<ElementoBusca*> vizinhos;
	while(!OpenSet.empty()){

		int lowest_f = 0;
		for(unsigned int k = 0; k < OpenSet.size(); k++){
			if(get_at(OpenSet,k)->f < get_at(OpenSet,lowest_f)->f){
				lowest_f = k;
			}
		}

		current = get_at(OpenSet,lowest_f);
		OpenSet.remove(current);
		ClosedSet.push_back(current);

		//Checando os vizinhos
		vizinhos = this->calcula_vizinhos(current);

		for(unsigned int k = 0; k < vizinhos.size(); k++){
			vizinho = get_at(vizinhos, k);
			//se o vizinho nao esta no closedSet
			bool found_closed = (std::find(ClosedSet.begin(), ClosedSet.end(), vizinho) != ClosedSet.end());
			if(!found_closed && !vizinho->obstacle){
				float temp = current->g + 1;
				bool found_open = (std::find(OpenSet.begin(), OpenSet.end(), vizinho) != OpenSet.end());
				if(found_open){
					if(temp < vizinho->g){
						vizinho->g = temp;
					}
				}else{
					vizinho->g = temp;
					OpenSet.push_back(vizinho);
				}

				vizinho->h = abs(goal_x - current->i) + abs(goal_y - current->j);
				vizinho->f = vizinho->h + vizinho->g;
			}
		}
		

	}


}