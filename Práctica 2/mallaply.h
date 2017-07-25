
#ifndef MALLAPLY_H
#define MALLAPLY_H

#include <vector>
#include "tuplas.hpp"

class MallaPLY{
   private:
   	//vectores de vertices y caras
	   std::vector<Tupla3f> vert;
	   std::vector<Tupla3i> car;
	   //vectores normales vertices y caras
	   std::vector<Tupla3f> norm_car;
 	   std::vector<Tupla3f> norm_vert;
 	   //numero de vertices y caras
 	   int num_car;
	   int num_vert;

   public:
   	//constructores
	   MallaPLY();
	   MallaPLY(std::vector<float> vertices_ply, std::vector<int> caras_ply);
	   //funciones dibujado
	   void Dibujar_Puntos();
	   void Dibujar_Lineas();
	   void Dibujar_Solido();
	   void Dibujar_Ajedrez();
	   void Dibujar_Normales_Colores();
	   void Dibujar_Normales_Lineas();
	   //funciones calculo normales
		void CalcularNormalesCaras();
		void CalcularNormalesVertices();

};


#endif
