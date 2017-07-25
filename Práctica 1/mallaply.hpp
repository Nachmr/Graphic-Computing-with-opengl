
#ifndef MALLAPLY_HPP
#define MALLAPLY_HPP

#include <vector>
#include "tuplas.hpp"

struct MallaPLY{
   std::vector<Tupla3f> vert;
   std::vector<Tupla3i> car;

   inline MallaPLY( std::vector<float> vertices_ply, std::vector<int> caras_ply);
};

inline MallaPLY::MallaPLY(std::vector<float> vertices_ply, std::vector<int> caras_ply){ //constructor que carga en la ED los vertices y las caras

   for(unsigned int i=0;i<vertices_ply.size();i=i+3)  //almaceno las coordenadas de los vertices
   {
      Tupla3f x(vertices_ply.at(i),vertices_ply.at(i+1),vertices_ply.at(i+2)); // estructura que almacena las una tupla de vertices
      this->vert.push_back(x); // incluimos en el vector de vertices de la estructura las coordenadas adquiridas
   }


   for(unsigned int i=0; i<caras_ply.size(); i=i+3) // almaceno los valores de las caras de los triangulos
   {
      Tupla3i y;
      y.idx[0] = caras_ply[i];
      y.idx[1] = caras_ply[i+1];
      y.idx[2] = caras_ply[i+2]; // estructura que almacena las tuplas carastuplas 
      this->car.push_back(y); // incluimos en el vector de caras de la estructura las coordenadas adquiridas
   }
}

#endif