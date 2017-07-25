
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

MallaPLY::MallaPLY(std::vector<float> vertices_ply, std::vector<int> caras_ply){ //constructor que carga en la ED los vertices y las caras
   CError();

   for(int i=0;i<vertices_ply.size();i=i+3)  //almaceno las coordenadas de los vertices
   {
      Tupla3f x(vertices_ply.at(i),vertices_ply.at(i+1),vertices_ply.at(i+2)); // estructura que almacena las una tupla de vertices
      vert.push_back(x); // incluimos en el vector de vertices de la estructura las coordenadas adquiridas
   }


   for(int i=0; i<caras_ply.size(); i=i+3) // almaceno los valores de las caras de los triangulos
   {
      Tupla3i y = {caras_ply.at(i+0),caras_ply.at(i+1),caras_ply.at(i+2)}; // estructura que almacena las tuplas carastuplas 
      car.push_back(y); // incluimos en el vector de caras de la estructura las coordenadas adquiridas
   }
   
   num_vert = vert.size();
   num_car = car.size();

   CalcularNormalesCaras();
   CalcularNormalesVertices();
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

   glColor3f(1,0,0);

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

void MallaPLY::Dibujar_Normales_Lineas(){
	this -> Dibujar_Ajedrez();
/*	glBegin(GL_LINES);
        glColor3f(0,0,1);
        for (int i = 0; i < num_vert-1; i++)
        {
         glVertex3f(vert.at(i).coo[0], vert.at(i+1).coo[1], vert.at(i+2).coo[2]);
         glVertex3f((norm_vert.at(i).coo[0]) + (norm_vert.at(i).coo[0]), (norm_vert.at(i+1).coo[1]) + (norm_vert.at(i+1).coo[1]) , (norm_vert.at(i+2).coo[2]) + (norm_vert.at(i+1).coo[2]));
        }
    glEnd();*/







     glPolygonMode(GL_FRONT, GL_FILL);
	 glPolygonMode(GL_BACK, GL_LINE);
	  
	 // dibujado de las normales de vértices
	 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	 
	 glColor3f(0,0,0);
	 glBegin(GL_LINES);
	 for(unsigned i = 0; i<num_vert; ++i){
		 glVertex3f(vert[i].coo[0], vert[i].coo[1], vert[i].coo[2] );
		 glVertex3f(norm_vert[i].coo[0]+vert[i].coo[0], norm_vert[i].coo[1]+vert[i].coo[1], norm_vert[i].coo[2] + vert[i].coo[2]);
	 }
	 glEnd();

}
