
#include "mallaply.h"
#include "error-ogl.hpp"
#include <GL/glut.h>
#include <iostream>


using namespace std;

//***********************************
// Funciones gestión de normales
//***********************************
void MallaPLY::CalcularNormalesCaras(){
   CError();
   for(int i=0; i < num_car; i++){
      //Vectores
      Tupla3f AB, BC, N; 
      //Puntos
      Tupla3f A, B, C;

      A = vert.at(car[i].idx[0]);
      B = vert.at(car[i].idx[1]);
      C = vert.at(car[i].idx[2]);

      AB = B - A;
      BC = C - B;

      N = AB * BC; //producto vectorial

      normalized(N); //normalizacion
      norm_car.push_back(N);
   }

   CError();
}

void MallaPLY::CalcularNormalesVertices(){
   CError();

   norm_vert = vector<Tupla3f>(num_car, Tupla3f(0,0,0)); //inicializamos el vector de normales de vertices

   for(int i = 0; i<num_car; ++i){
      norm_vert.at(car.at(i).idx[0]) = norm_vert.at(car.at(i).idx[0]) + norm_car.at(i); //rellenamos el vector de normales de vertices
      norm_vert.at(car.at(i).idx[1]) = norm_vert.at(car.at(i).idx[1]) + norm_car.at(i);
      norm_vert.at(car.at(i).idx[2]) = norm_vert.at(car.at(i).idx[2]) + norm_car.at(i);
   }
   
  //Normalizamos los vectores
  for (int i = 0; i < num_vert; ++i){
      norm_vert.at(i) = normalized(norm_vert.at(i));
  }
  CError();
}


//***********************************
// Constructores
//***********************************

MallaPLY::MallaPLY(){
    car[0].idx[0] = 0;
    vert[0].coo[0] = 0;
    num_vert = vert.size()/3;
    num_car = car.size()/3;
}

MallaPLY::MallaPLY(vector<float> ver, vector<int> tri ){ //constructor que carga en la ED los vertices y las caras
   CError();

   num_vert = ver.size() / 3 ;
   num_car = tri.size() / 3 ;
   
   // Escribe vértices en el vector.
   for( unsigned i = 0 ; i < num_vert * 3 ; i = 3 + i )
   {
       Tupla3f vi( ver.at( i + 0 ), ver.at( i + 1 ), ver.at( i + 2 ) ) ;
       vert.push_back( vi ) ;

	   
   }
   
   // Escribe índices en el vector.
   for( unsigned i = 0 ; i < num_car * 3 ; i = 3 + i )
   {
       Tupla3i indexi = { tri.at( i + 0 ), tri.at( i + 1 ) , tri.at( i + 2 ) } ;
       car.push_back( indexi ) ;
   }	
   


   
   CError();

}



	
//***********************************
// Funciones de dibujado
//***********************************
void MallaPLY::Dibujar_Puntos(){

   CError();

   glColor3f(1,0,0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
   // habilitar el uso de puntero a tabla de coordenadas
   glEnableClientState( GL_VERTEX_ARRAY );
   // proporcionar el puntero (tri) la tabla de coordenadas
   glVertexPointer( 3, GL_FLOAT, 0,&(vert.at(0).coo));
   glDrawElements(GL_POINTS,3*car.size(), GL_UNSIGNED_INT,&(car.at(0)).idx[0]);

   CError();
}

void MallaPLY::Dibujar_Lineas(){
   CError();

   glColor3f(1,0,0);

   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   // habilitar el uso de puntero a tabla de coordenadas
   glEnableClientState( GL_VERTEX_ARRAY );
   // proporcionar el puntero (tri) la tabla de coordenadas
   glVertexPointer( 3, GL_FLOAT, 0,&(vert.at(0).coo));
   glDrawElements(GL_TRIANGLES,3*car.size(),GL_UNSIGNED_INT,&(car.at(0)).idx[0]);

   CError();
}

void MallaPLY::Dibujar_Solido(){
   CError();

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   // habilitar el uso de puntero a tabla de coordenadas
   glEnableClientState( GL_VERTEX_ARRAY );
   // proporcionar el puntero (tri) la tabla de coordenadas
   glVertexPointer(3, GL_FLOAT, 0,&(vert.at(0).coo));
   glDrawElements(GL_TRIANGLES,3*car.size(),GL_UNSIGNED_INT,&(car.at(0)).idx[0]);

   CError();
}

void MallaPLY::Dibujar_Ajedrez(){
   CError();
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_TRIANGLES);

   for(int i=0; i < car.size(); i++){
      (i%2==0) ? (glColor3f(0,1,0)) : (glColor3f(0,0,1)); // Alternamos colores dependiendo si el contador es par o impar
      int ver_x = car[i].idx[0]; //Obtenemos el primer vertice de un triangulos
      glVertex3f(vert[ver_x].coo[0],vert[ver_x].coo[1],vert[ver_x].coo[2]); //accedemos a sus coordenadas y las dibujamos
      int ver_y=car[i].idx[1];
      glVertex3f(vert[ver_y].coo[0],vert[ver_y].coo[1],vert[ver_y].coo[2]);
      int ver_z=car[i].idx[2];
      glVertex3f(vert[ver_z].coo[0],vert[ver_z].coo[1],vert[ver_z].coo[2]);
   }
   
   glEnd();

   CError();
}

void MallaPLY::Dibujar_Normales_Colores() {
	CalcularNormalesCaras();
	CalcularNormalesVertices();
   CError();
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glBegin( GL_TRIANGLES) ;
      for( int i = 0 ; i < num_car ; ++i){
         int indice = car.at(i).idx[0]; // indice es cada uno de los tres indicies asociados a una cara
         glColor3f(fabs(norm_vert.at(indice).coo[0]), fabs(norm_vert.at(indice).coo[1]),fabs(norm_vert.at(indice).coo[2]));
         glVertex3fv(&(vert.at(indice).coo[0]));  

         indice = car.at(i).idx[1];
         glColor3f(fabs(norm_vert.at(indice).coo[0]), fabs(norm_vert.at(indice).coo[1]),fabs(norm_vert.at(indice).coo[2]));
         glVertex3fv(&(vert.at(indice).coo[0]));  

         indice = car.at(i).idx[2];
         glColor3f(fabs(norm_vert.at(indice).coo[0]), fabs(norm_vert.at(indice).coo[1]),fabs(norm_vert.at(indice).coo[2]));
         glVertex3fv(&(vert.at(indice).coo[0]));   
      }
   glEnd();
   glShadeModel(GL_SMOOTH); //método que utiliza OpenGL para rellenar de color los polígonos con  GL_SMOOTH, se rellena interpolando los colores activos en la definición de cada vértice.

   CError();
}


void MallaPLY::dibujar(char visualizacion, GLfloat c[][3]){
    switch (visualizacion){

		case 'p':
			glPointSize(2);
			glColor3f(c[0][0],c[0][1],c[0][2]);
			Dibujar_Puntos();
			break;
		case 'l':
			glColor3f(c[0][0],c[0][1],c[0][2]);
			Dibujar_Lineas();
			break;
		case 's':			
			glColor3f(c[0][0],c[0][1],c[0][2]);
			Dibujar_Solido();
			break;	
		case 'a':
			Dibujar_Ajedrez();
			break;
		default:
			glColor3f(c[0][0],c[0][1],c[0][2]);
			Dibujar_Solido();
			
    }
}
