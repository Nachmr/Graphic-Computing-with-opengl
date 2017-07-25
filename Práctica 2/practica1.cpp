// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************


#include <GL/glut.h>
#include <iostream>
#include <vector>

#include "error-ogl.hpp"
#include "file_ply_stl.hpp" //Funciones para leer archivo ply
#include "tuplas.hpp"   // Tupla3f, Tupla4f, Tupla3i
#include "practica1.hpp"
#include "mallaply.h"



MallaPLY *malla; //Declaracion de un puntero de tipo malla


// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P1_Inicializar(int argc, char *argv[])
{
   //Comprobación de parámetros y lectura fichero PLY

   if(argc > 1){
      if (argc < 3){
         std::cout << "Numero de parámetros incorrectos. Ejecución: \n\t" << argv[0] << " Practica 1:<fichero.ply> Practica2:<perfil.ply> <nnum_rotaciones> "<< std::endl;
         exit(-1);
      }

      std::vector<float> vertices_ply; //vector que contendrá los vértices del fichero ply
      std::vector<int> caras_ply; //vector que contendrá las caras del fichero ply

      ply::read(argv[1],vertices_ply,caras_ply);   //Leemos el fichero ply con el constructor con parámetros
      malla = new MallaPLY(vertices_ply,caras_ply);

      std::cout << "Indique con las teclas 1(puntos), 2(alambre), 3(solido) o 4(ajedrez) la forma de dibujado." << std::endl;

   }
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos

void P1_DibujarObjetos(int modo) 
{
   switch(modo){
      case 1 : //Dibujado con punto
		 malla->Dibujar_Puntos();
         break;
      case 2 : //Dibujado con alambre
		 malla->Dibujar_Lineas();
         break;
      case 3: // Dibujado solido
		 malla->Dibujar_Solido();
         break;
      case 4: // Dibujado ajedrez
		 malla->Dibujar_Ajedrez();
      case 5: // Dibujado normales
       malla->Dibujar_Normales_Colores();
      case 6: // Dibujado normales
       malla->Dibujar_Normales_Lineas();
         break;
   }
}
