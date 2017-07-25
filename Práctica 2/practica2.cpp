// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 2  (implementación)
// **
// *********************************************************************


#include <GL/glut.h>
#include <iostream>
#include <vector>

#include "error-ogl.hpp"
#include "file_ply_stl.hpp" //Funciones para leer archivo ply
#include "tuplas.hpp"   // Tupla3f, Tupla4f, Tupla3i
#include "practica2.hpp"

#include "mallaply.h"


// *********************************************************************
// **
// ** Variables globales
// **
// *********************************************************************


MallaPLY * malla2; //Declaracion de un puntero de tipo malla

const double PI=3.14159265;

std::vector<float> vertices_ply;
std::vector<int> caras_ply;


// *********************************************************************
// **
// ** Función para crear las tapas teniendo en cuenta si estan o no en los ejes
// **
// *********************************************************************


void CrearTapas( std::vector<float> & dest, std::vector<float> orig, std::vector<int> & caras,  Tupla3f sup, Tupla3f inf){
   
   dest.push_back(sup.coo[0]);
   dest.push_back(sup.coo[1]);
   dest.push_back(sup.coo[2]);
   dest.push_back(inf.coo[0]);
   dest.push_back(inf.coo[1]);
   dest.push_back(inf.coo[2]);

   int numvertorig = orig.size()/3;
   int numvertdest = dest.size()/3; 


   for (int i = 0; i < numvertdest-numvertorig-2; i = i+numvertorig){
      caras.push_back(i);
      caras.push_back(i+numvertorig);
      caras.push_back(numvertdest-2);
   }

   //Juntas caras del ultimo y el primer perfil
   caras.push_back(numvertdest-2-numvertorig); 
   caras.push_back(0);
   caras.push_back(numvertdest-2);

   for (int i = numvertorig-1; i < numvertdest-numvertorig-2; i = i+numvertorig){
      caras.push_back(i);
      caras.push_back(numvertdest-1);
      caras.push_back(i+numvertorig);
   }

   //Juntas vertices del ultimo y el primer perfil
   caras.push_back(numvertdest-3);
   caras.push_back(numvertdest-1);
   caras.push_back(numvertorig-1);


}


// *********************************************************************
// **
// ** Funcion para generar el perfil (menos las tapas)
// **
// *********************************************************************


void GenerarPerfil( std::vector<float> & dest, std::vector<float> & orig, int n, std::vector<int> & caras ){
 
   double radianes;
   for(int i = 0; i < n; ++i){
      for(int j = 0; j< orig.size(); j=j+3){
         radianes = 2*PI*i/n;
         float cosen = cos(radianes), sen = sin(radianes);
         dest.push_back(orig.at(j)*cosen + orig.at(j+2)*sen);
         dest.push_back(orig.at(j+1));
         dest.push_back(orig.at(j+2)*cosen-orig.at(j)*sen);
      }
   }

   int numvertorig = orig.size()/3; //vertices del perfil original
   int vertytapas = dest.size()/3; // vertices totales incl. tapas

   for(int i = 0; i<vertytapas; ++i){//para cada vertice del cuerpo
      if(i%numvertorig == 0){//toca con la cara superior
         caras.push_back(i); 
         caras.push_back(i+1);
         caras.push_back((i+numvertorig)%vertytapas); //contiguo del perfil siguiente
      }
      else if(i%numvertorig == (numvertorig-1)){//toca la tapa inferior
         caras.push_back(i);
         caras.push_back((i+numvertorig)%vertytapas);
         caras.push_back((i+numvertorig)%vertytapas-1);
      }
      else{
         caras.push_back(i);
         caras.push_back((i+numvertorig)%vertytapas);
         caras.push_back((i+numvertorig)%vertytapas-1);
         caras.push_back(i); //él mismo en el siguiente triangulo
         caras.push_back(i+1);// el siguiente del perfil
         caras.push_back((i+numvertorig)%vertytapas); //contiguo del perfil siguiente
      }
   }
}


// *********************************************************************
// **
// ** Función para generar el objeto de revolución al partir del perfil. Usa las funciones CrearPerfil y CrearTapas
// **
// *********************************************************************


void Revolucion(std::vector<float> & dest, std::vector<float> & orig,  std::vector<int> & caras, int n){
   if(orig.at(2)==0 && orig.at(0)==0){//El vertice superior esta en el ply
      
      if(orig.at(orig.size()-3) == 0 && orig.at(orig.size()-1) == 0){//el inferior esta en el ply
         //Se extraen los vertices
         Tupla3f sup(orig.at(0),orig.at(1),orig.at(2));
         Tupla3f inf(orig.at(orig.size()-3),orig.at(orig.size()-2),orig.at(orig.size()-1));

         orig.erase(orig.begin(),orig.begin()+3);
         for(unsigned i=0;i<3;++i)    orig.pop_back();

         GenerarPerfil(dest,orig,n,caras);
         CrearTapas(dest, orig, caras, sup, inf);
      }

      else{
         Tupla3f sup(orig.at(0),orig.at(1),orig.at(2));
         Tupla3f inf(0,orig.at(orig.size()-2),0);

         orig.erase(orig.begin(),orig.begin()+3);

         GenerarPerfil(dest,orig,n,caras);
         CrearTapas(dest,orig, caras,sup,inf);
      }

   }
   else{
      if( orig.at(orig.size()-1) == 0 && orig.at(orig.size()-3) == 0 ){//el vertice inferior  si esta en el eje Y
         Tupla3f sup(0,orig.at(1),0);
         Tupla3f inf(orig.at(orig.size()-3),orig.at(orig.size()-2),orig.at(orig.size()-1));

         for(int i=0;i<3;++i){ 
            orig.pop_back();
         }

         GenerarPerfil(dest,orig,n,caras);
         CrearTapas(dest, orig, caras, sup, inf);
      }else{ //no tiene vertices en Y

         Tupla3f sup(0,orig.at(1),0);
         Tupla3f inf(0,orig.at(orig.size()-2),0);

         GenerarPerfil(dest,orig,n,caras);
         CrearTapas(dest, orig, caras, sup, inf);
      }
   }
}

// *********************************************************************
// **
// ** Función de inicialización
// **
// *********************************************************************

void P2_Inicializar(int argc, char *argv[])
{
   //Comprobación de parámetros y lectura fichero PLY
   if(argc>1){
      ply::read_vertices(argv[2],vertices_ply);//leemos el ply
      
      std::vector<int> caras_revolucion;
      std::vector<float> vertices_revolucion;
      Revolucion(vertices_revolucion, vertices_ply,  caras_revolucion, atoi(argv[3]));
       
      malla2 = new MallaPLY(vertices_revolucion,caras_revolucion); //creamos la MallaPly con los datos leídos
        
      std::cout << "Indique con las teclas 1(puntos), 2(alambre), 3(solido), 4(ajedrez) o 5(normales) la forma de dibujado." << std::endl;
   }
}

// *********************************************************************
// **
// ** Función gestora de modo de dibujado
// **
// *********************************************************************


void P2_DibujarObjetos(int modo) 
{
   switch(modo){
      case 1 : //Dibujado con punto
		 malla2->Dibujar_Puntos();
         break;
      case 2 : //Dibujado con alambre
		 malla2->Dibujar_Lineas();
         break;
      case 3: // Dibujado solido
		 malla2->Dibujar_Solido();
         break;
      case 4: // Dibujado ajedrez
		 malla2->Dibujar_Ajedrez();
         break;
      case 5: // Dibujado normales por colores
       malla2->Dibujar_Normales_Colores();
         break;
      case 6: // Dibujado normales por lineas
       malla2->Dibujar_Normales_Lineas();
         break;
      default:
         malla2->Dibujar_Puntos();
         break;
   }
}
