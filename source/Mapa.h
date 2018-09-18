#ifndef MAPA_H_
#define MAPA_H_
#define TAMANHO 50 

class Mapa {
public:

	int mapa[TAMANHO][TAMANHO];//posicao

	Mapa();//Carrega o mapa de um arquivo txt para a matriz
	~Mapa();

	//Funcoes para obter o tipo de objeto em uma posicao da matriz
	//Criacao de uma lista de objetos do mapa e registro no renderizador
};



#endif