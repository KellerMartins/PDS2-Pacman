#ifndef MAPA_H_
#define MAPA_H_
#define LINHAS 13
#define COLUNAS 15
#include <string>

class Mapa {
public:

	int mapa[LINHAS][COLUNAS];

	Mapa(std::string arq);//Carrega o mapa de um arquivo txt para a matriz
	~Mapa();

	//Funcoes para obter o tipo de objeto em uma posicao da matriz
	//Criacao de uma lista de objetos do mapa e registro no renderizador
};



#endif