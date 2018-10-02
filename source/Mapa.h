#ifndef MAPA_H_
#define MAPA_H_

#include "object3D.h"
#include "rendering.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#define LINHAS 13
#define COLUNAS 15

enum ElementoMapa{Vazio, Parede};

class Mapa {
private:
	ElementoMapa _mapa[COLUNAS][LINHAS];
	std::vector<Object3D*> _objetosMapa;
	
	void RegistraMapaRenderizavel();
	void DesregistrarMapaRenderizavel();
public:

	Mapa(std::string arq);//Carrega o mapa de um arquivo txt para a matriz
	~Mapa();
	
	Object3D* GetObjetoMapa(int indiceObjeto);
	int GetNumeroObjetosMapa();

	ElementoMapa GetElementoMapa(unsigned int x, unsigned int y);
};



#endif