// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************


#include <GL/glut.h>

#include "error-ogl.hpp"
#include "practica1.hpp"
#include <iostream>

//includes para lectura de ply

#include <vector>
#include "file_ply_stl.hpp"

//include para mi clase MallaPly, tambien incluye el tuplas.hpp
#include "MallaPly.h"


using namespace std;
// ---------------------------------------------------------------------
// coordenadas de los vértices del cubo:

const unsigned num_verts_cubo = 8 ;

GLfloat coords_verts_cubo[num_verts_cubo][3] = 
   {  { -0.5, -0.5, +0.5 } ,
      { +0.5, -0.5, +0.5 } ,
      { +0.5, +0.5, +0.5 } ,
      { -0.5, +0.5, +0.5 } ,
      { -0.5, -0.5, -0.5 } ,
      { +0.5, -0.5, -0.5 } ,
      { +0.5, +0.5, -0.5 } ,
      { -0.5, +0.5, -0.5 }
   };
   
vector<float> verts_plyP1;
vector<int> caras_plyP1;

//puntero global a la malla
MallaPly * meshP1;


// ---------------------------------------------------------------------
//  dibujar vértices del cubo en modo inmediato con begin/end

void DibujarCuboPuntos()
{
   glColor3f( 0.10, 0.15, 0.40 );
   glPointSize(4);

   glBegin(GL_POINTS);
   for ( unsigned i= 0 ; i < num_verts_cubo ; i++ )
      glVertex3fv( &( coords_verts_cubo[i][0] ) );
   glEnd();
   
}

// ---------------------------------------------------------------------
//  dibujar vertices en modo vector

void DibujarCuboPuntosVector(){
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, coords_verts_cubo);
  glDrawArrays(GL_POINTS,0,24);
  
}

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e 
// incializado OpenGL. El PLY se debe cargar aquí.

void P1_Inicializar( int argc, char *argv[] )
{
  if(argc>1){
    ply::read(argv[1],verts_plyP1, caras_plyP1);//leemos el ply
    meshP1 = new MallaPly(verts_plyP1,caras_plyP1); //creamos la MallaPly con los datos leídos
     
    using namespace std ;
    cout << "Pulsa una tecla para elegir modo de visualización o dibujado:\n "
	  << "1: Modo Puntos\t 2:Modo Alambre\t 3:Modo Solido\t 4:Modo Ajedrez\n"
	  << "5: Dibujado con glBegin/glEnd\t 6: Dibujado con glDrawElements"<< endl ;
  }

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos
// Parámetros:
// int modo_vis: modo de visualización:
//      1: Puntos, 2: Alambre, 3: Sólido, 4: Ajedrez,
// int modo_dib: modo de dibujado:
// 	1: glBegin()/glEnd(), 2: glDrawElements()


void P1_DibujarObjetos(int modo_vis, int modo_dib) 
{
  if(verts_plyP1.size()==0){//Si no se ha leído un ply...
    DibujarCuboPuntosVector(); //Se dibuja el cubo unidad
  }
  else{	//en caso contrario...
    switch(modo_vis){//Miramos el modo de visualización...
      case 1: //Modo puntos
	if(modo_dib==1) meshP1->DibujarBE_vertices();//glBegin/glEnd
	else meshP1->DibujarDE_vertices();		  //glDrawElements
	break;
      case 2://Modo alambre
	if(modo_dib==1) meshP1->DibujarBE_alambre();
	else meshP1->DibujarDE_alambre();	
	break;
      case 3://Modo solido
	if(modo_dib==1) meshP1->DibujarBE_solido();
	else meshP1->DibujarDE_solido();	
	break;
      case 4://Modo ajedrez
	if(modo_dib==1) meshP1->DibujarBE_ajedrez();
	else meshP1->DibujarDE_ajedrez();
	break;
      default:
	cout << "Ninguna opcion válida... Dibujando en modo puntos" << endl;
	meshP1->DibujarDE_vertices();
	break;
    }
  }
}