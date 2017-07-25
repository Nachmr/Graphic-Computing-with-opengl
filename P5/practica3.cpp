// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Carlos Ureña Almagro
// ** 
// ** Práctica 3  (implementación)
// *********************************************************************

#include <GL/glut.h>
#include "error-ogl.hpp"
#include "practica3.hpp"
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

MallaPLY *esfera, *cubo, *circunsferencia;
vector<float> verticesEsf, verticesCub, verticesCir;
vector<int> carasEsf, carasCub, carasCir;

/*Colores para los elementos*/
GLfloat rojo[1][3] = {1.0,0.0,0.0};
GLfloat verde[1][3] = {0.0,1.0,0.0};
GLfloat azul[1][3] = {0.0,0.0,1.0};
GLfloat amarillo[1][3] = {1.0,1.0,0.0};
GLfloat magenta[1][3] = {1.0,0.0,1.0};
GLfloat cyan[1][3] = {0.0,1.0,1.0};
GLfloat blanco[1][3] = {1.0,1.0,1.0};

float giroCamaraVertical = 0;
float giroVisera = 0;
float giroCamaraHorizontal = 0;
char P3_modo='s';


//Funcion que decrementa el angulo del cuerpo de la camara
void P3_girarCuerpo_arriba(){
	if(giroCamaraHorizontal>-13){
		giroCamaraHorizontal = giroCamaraHorizontal - 1;
	}
	
}

//Funcion que incrementa el angulo del cuerpo de la camara
void P3_girarCuerpo_abajo(){
	if(giroCamaraHorizontal<13){
		giroCamaraHorizontal = giroCamaraHorizontal + 1;
	}
}


//Funcion que incrementa el angulo para girar la camara a la derecha
void P3_girarCamara_der(){
	if(giroCamaraVertical>-12.5){
		giroCamaraVertical = giroCamaraVertical - 1;
	}
}

//Funcion que incrementa el angulo para girar la camara a la izquierda
void P3_girarCamara_izq(){
	if(giroCamaraVertical<12.5){
		giroCamaraVertical = giroCamaraVertical + 1;
	}
}

//Funcion que incrementa el angulo para subir la visera
void P3_girarVisera_arriba(){
	if(giroVisera>-15){
		giroVisera = giroVisera - 1;
	}
}

//Funcion que decremanta el angulo para bajar la visera
void P3_girarVisera_abajo(){
	if(giroVisera<15){
		giroVisera = giroVisera + 1;
	}
}




bool P3_FGE_TeclaNormal( unsigned char tecla, int x_raton, int y_raton, char &P3_modo)
{
	CError();
	switch (tecla)
	{      			
		case 'Q' :
			exit( 0 );
			break ;      
			// Visualizar en modo 'Ajedrez'	
		case 'p':
			P3_modo = 'p' ;
			break;
			// Visualizar en modo 'líneas/aristas'
		case 'l':
			P3_modo = 'l' ;
			break;
			// Visualizar en modo 'Sólido'
		case 'a':
			P3_modo = 'a' ;
			break;
			// Visualizar en modo 'Puntos'
		case 's':
			P3_modo = 's' ;
			break;
			// Modifica primer grado de libertad del modelo jerárquico (aumenta/disminuye)
		case 'Z':
			P3_girarCuerpo_arriba(); 
			break;	
		case 'z':
			P3_girarCuerpo_abajo();
			break;
			// Modifica segundo grado de libertad del modelo jerárquico (aumenta/disminuye)
		case 'X':
			P3_girarCamara_der(); 
			break;
		case 'x':
			P3_girarCamara_izq();
			break;
			// Modifica tercer grado de libertad del modelo jerárquico (aumenta/disminuye)
		case 'C':
			P3_girarVisera_arriba(); 
			break;
		case 'c':
			P3_girarVisera_abajo(); 
			break;
		default:
			return false ;
	}   
	// si se ha cambiado algo, forzar evento de redibujado
	return true;
	
}

void P3_Inicializar()
{   

  ply::read("componentes/esfera.ply",verticesEsf,carasEsf);
  esfera = new MallaPLY(verticesEsf,carasEsf);
  
  ply::read("componentes/cubo.ply",verticesCub,carasCub);
  cubo = new MallaPLY(verticesCub,carasCub);
  
  ply::read("componentes/circunsferencia.ply",verticesCir,carasCir);
  circunsferencia = new MallaPLY(verticesCir,carasCir);
  
}



void P3_DibujarObjetos(char visualizacion){

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity;
  glPushMatrix();//Camara

    //Cuadrilatero que representa la pared de la camara
  glPushMatrix();
	glTranslatef(7.5,-2.5,0);
	glRotatef(90,1,0,0);
	glRotatef(90,0,1,0);
	glScalef(4,4,0.1);
	cubo->dibujar(visualizacion,cyan);
  
  glPopMatrix();
    //Camara
	glPushMatrix();
  
			glRotatef(giroCamaraHorizontal,0,0,1);
			glRotatef(giroCamaraVertical,0,1,0);
		// Cuerpo de la camara
		glPushMatrix();

			glTranslatef(0,2,0);
			glScalef(10,3,3);
			cubo->dibujar(visualizacion,rojo);
		glPopMatrix();
		
		//Objetivo
		glPushMatrix();
			glTranslatef(0,2,0);
 			glRotatef(90,1,0,0);

			glRotatef(90,0,0,1);
			glScalef(.5,.5,.5);
			circunsferencia->dibujar(visualizacion,azul);
		glPopMatrix();
		
		//Visera
		glPushMatrix();
			glTranslatef(-5.6,3.4,0);
			glRotatef(giroVisera,0,0,1);
 			glRotatef(90,1,0,0);
			glScalef(1.3,3,0.2);
			cubo->dibujar(visualizacion,verde);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		
		//Brazo1
		glPushMatrix();
			glTranslatef(4,0,0);
			glScalef(1,4,1);
			cubo->dibujar(visualizacion,blanco);
		glPopMatrix();

		//Brazo2
		glPushMatrix();
			glTranslatef(5.5,-2.5,0);
			glRotatef(90,1,0,0);
			glRotatef(90,0,0,1);
			glScalef(1,4,1);
			cubo->dibujar(visualizacion,amarillo);
		glPopMatrix();
		
	glPopMatrix();

      
  glPopMatrix();
     CError();

}






