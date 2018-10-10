#ifndef MAPA_H_
#define MAPA_H_

#include "object3D.h"
#include "rendering.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "gameEvents.h"

#define LINHAS 13
#define COLUNAS 15

enum ElementoMapa{Vazio, Parede};

class Mapa : public GameEvents{
	private:
		ElementoMapa _mapa[COLUNAS][LINHAS];
		std::vector<Object3D*> _objetosMapa;
		
		void RegistraMapaRenderizavel();
		void DesregistraMapaRenderizavel();
	public:

		Mapa(std::string arq);//Carrega o mapa de um arquivo txt para a matriz
		~Mapa();
		
		Object3D* GetObjetoMapa(int indiceObjeto);
		int GetNumeroObjetosMapa();

		ElementoMapa GetElementoMapa(unsigned int x, unsigned int y);

		virtual void OnUpdate() override;
};



#endif