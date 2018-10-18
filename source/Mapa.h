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

enum ElementoMapa{Vazio, Parede, Ponto, Especial};

class Mapa : public GameEvents{
	private:
		static Mapa _mapaGlobal;

		ElementoMapa _mapa[COLUNAS][LINHAS];
		std::vector<Object3D*> _objetosMapa;
		
		void RegistraMapaRenderizavel();
		void DesregistraMapaRenderizavel();

		Mapa() {};
	public:
		
		~Mapa();

		static void CarregaArquivo(std::string arq);
		static ElementoMapa GetElementoMapa(unsigned int x, unsigned int y);
		static void RemoveElementoMapa(unsigned int x, unsigned int y);

		virtual void OnUpdate() override;
};



#endif