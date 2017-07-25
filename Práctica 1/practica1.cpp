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
#include "mallaply.hpp"



MallaPLY *malla; //Declaracion de un puntero de tipo malla

void P1_DibujarPunt(MallaPLY *malla){

   CError();

   glColor3f(1,0,0);
   //glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
   // habilitar el uso de puntero a tabla de coordenadas
   glEnableClientState( GL_VERTEX_ARRAY );
   // proporcionar el puntero (tri) la tabla de coordenadas
   glVertexPointer( 3, GL_FLOAT, 0,&(malla->vert.at(0)).coo[0]);
   glDrawElements(GL_POINTS,3*(malla->car.size()), GL_UNSIGNED_INT,&(malla->car.at(0)).idx[0]);

   CError();
}

void P1_DibujarLin(MallaPLY *malla){

   CError();

   glColor3f(1,0,0);


   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   // habilitar el uso de puntero a tabla de coordenadas
   glEnableClientState( GL_VERTEX_ARRAY );
   // proporcionar el puntero (tri) la tabla de coordenadas
   glVertexPointer( 3, GL_FLOAT, 0,&(malla->vert.at(0)).coo[0]);
   glDrawElements(GL_TRIANGLES,3*(malla->car.size()),GL_UNSIGNED_INT,&(malla->car.at(0)).idx[0]);

   CError();
}

void P1_DibujarSol(MallaPLY *malla){

   CError();

   glColor3f(1,0,0);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // habilitar el uso de puntero a tabla de coordenadas
   glEnableClientState( GL_VERTEX_ARRAY );
   // proporcionar el puntero (tri) la tabla de coordenadas
   glVertexPointer(3, GL_FLOAT, 0,&(malla->vert.at(0)).coo[0]);
   glDrawElements(GL_TRIANGLES,3*(malla->car.size()),GL_UNSIGNED_INT,&(malla->car.at(0)).idx[0]);

   CError();
}

void P1_DibujarAje(MallaPLY *malla){

   CError();
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_TRIANGLES);
   
   for(unsigned int i=0; i < malla->car.size(); i++){
      (i%2==0) ? (glColor3f(0,1,0)) : (glColor3f(0,0,1)); // Alternamos colores dependiendo si el contador es par o impar
      int ver_x = malla->car[i].idx[0]; //Obtenemos el primer vertice de un triangulos
      glVertex3f(malla->vert[ver_x].coo[0],malla->vert[ver_x].coo[1],malla->vert[ver_x].coo[2]); //accedemos a sus coordenadas y las dibujamos
      int ver_y=malla->car[i].idx[1];
      glVertex3f(malla->vert[ver_y].coo[0],malla->vert[ver_y].coo[1],malla->vert[ver_y].coo[2]);
      int ver_z=malla->car[i].idx[2];
      glVertex3f(malla->vert[ver_z].coo[0],malla->vert[ver_z].coo[1],malla->vert[ver_z].coo[2]);
   }
   
   glEnd();

   CError();

}
// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P1_Inicializar(int argc, char *argv[])
{
   //Comprobación de parámetros y lectura fichero PLY


   if (argc != 2){
      std::cout << "Numero de parámetros incorrectos. Ejecución: \n\t" << argv[0] << " <fichero.ply> "<< std::endl;
      exit(-1);
   }

   std::vector<float> vertices_ply; //vector que contendrá los vértices del fichero ply
   std::vector<int> caras_ply; //vector que contendrá las caras del fichero ply

   ply::read(argv[1],vertices_ply,caras_ply);   //Leemos el fichero ply con el constructor con parámetros
   malla = new MallaPLY(vertices_ply,caras_ply);

   std::cout << "Indique con las teclas 1(puntos), 2(alambre), 3(solido) o 4(ajedrez) la forma de dibujado." << std::endl;


}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos

void P1_DibujarObjetos(int modo) 
{
   switch(modo){
      case 1 : //Dibujado con punto
         P1_DibujarPunt(malla);
         break;
      case 2 : //Dibujado con alambre
         P1_DibujarLin(malla);
         break;
      case 3: // Dibujado solido
         P1_DibujarSol(malla);
         break;
      case 4: // Dibujado ajedrez
         P1_DibujarAje(malla);
         break;
   }
}
