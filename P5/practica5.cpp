// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Carlos Ureña Almagro
// ** 
// ** Práctica 5  (implementación)
// ** Ignacio Martín Requena
// *********************************************************************

#include <GL/glut.h>
#include "error-ogl.hpp"
#include "practica1.hpp"
#include "practica2.hpp"
#include "practica3.hpp"
#include "practica5.hpp"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

bool estadoRaton[2]={0,0};
int xc=0, yc=0, xr = 0, yr = 0;
const unsigned int BUFFER_SIZE = 100;
unsigned int id = 0;
extern char modo;


// variables que definen la posicion de la camara en coordenadas polares
extern float 
camara_angulo_x ,   // angulo de rotación entorno al eje X
camara_angulo_y ;   // angulo de rotación entorno al eje Y

// variables que controlan la ventana y la transformacion de perspectiva
// variables que definen el view-frustum (zona visible del mundo)
extern float 
frustum_factor_escala ,  // factor de escala homogeneo que se aplica al frustum (sirve para alejar o acercar)
frustum_ancho ,          // ancho, en coordenadas del mundo
frustum_alto ,
frustum_dis_del ,        // distancia al plano de recorte delantero (near)
frustum_dis_tra ,        // distancia al plano de recorte trasero (far)
eye[] ,
at[] ;

// variables que determninan la posicion y tamaño de la ventana X
extern float 
ventana_pos_x  ,  // posicion (X) inicial de la ventana, en pixels 
ventana_pos_y  ,  // posicion (Y) inicial de la ventana, en pixels
ventana_tam_x  ,  // ancho inicial y actual de la ventana, en pixels
ventana_tam_y  ;  // alto inicial actual de la ventana, en pixels

// Inicializa la matriz GL_MODELVIEW 
void P5_fijarVista()
{
	// posicion del observador
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0);
	
	glRotatef(camara_angulo_x,1,0,0);
	glRotatef(camara_angulo_y,0,1,0);
	
}

void P5_fijarProyeccion(  )
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-frustum_ancho-3,frustum_ancho+3,-frustum_alto-3,frustum_alto+3,frustum_dis_del, frustum_dis_tra);
}


void P5_Inicializar( int argc, char *argv[] )
{   
	P1_Inicializar( argc, argv ) ;
	P2_Inicializar( argc, argv ) ;
	P3_Inicializar() ;	
}

void P5_DibujarObjetos( char modo ) 
{	
	
	glPushName(1);    		
	P3_DibujarObjetos( modo ) ;
	glTranslatef( -10, 0, 0 ) ;
	glPushName(2);
	P2_DibujarObjetos( modo ) ;
	glTranslatef( 18, 0, 0 ) ;
	glPushName(3);
	glTranslatef( 0, 5, 0 ) ;
	glScalef(0.01,0.01,0.01);
	P1_DibujarObjetos( modo ) ;		
	
}

void P5_Examinar( int x )
{
	switch( x )
	{
		case 1:
			cout << "ID 1: Galleon "<< id << endl;
			break;
		case 2:
			cout << "ID 2: Camara "<< id << endl;
			break;
		case 3:
			cout << "ID 3: Botella "<< id << endl;
			break;
		case -1:
			P5_DibujarObjetos ( modo ) ;
			break;        
	}    
}

GLuint Selection_buffer[BUFFER_SIZE];

void P5_ProcesarHits( GLint hits, GLuint buffer[] )
{
	unsigned int i, j;
	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;
	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) 
	{
		names = *ptr;
		ptr++;
		if (*ptr < minZ) 
		{
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}
		ptr += names+2;
	}	
	ptr = ptrNames;
	for (j = 0; j < numberOfNames; j++,ptr++) 
	{
		P5_Examinar( *ptr );
	}
	
}


void pick( unsigned int x, unsigned int y )
{    
	GLint Hits, Viewport[4];
	glGetIntegerv( GL_VIEWPORT, Viewport );
	glSelectBuffer( BUFFER_SIZE, Selection_buffer );
	glRenderMode(GL_SELECT);
	glInitNames();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluPickMatrix( x, Viewport[3] - y, 5.0, 5.0, Viewport ) ;
	glFrustum( -frustum_ancho, frustum_ancho, -frustum_alto, frustum_alto, frustum_dis_del, frustum_dis_tra ) ;
	P5_DibujarObjetos ( modo ) ;
	Hits = glRenderMode (GL_RENDER);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glFrustum( -frustum_ancho, frustum_ancho, -frustum_alto, frustum_alto, frustum_dis_del, frustum_dis_tra ) ;
	if ( Hits != 0 )
		P5_ProcesarHits( Hits, Selection_buffer ) ;
	P5_DibujarObjetos ( modo ) ;
	P5_Examinar( id ) ;    
}

bool P5_FGE_TeclaNormal( unsigned char tecla, int x_raton, int y_raton )
{
	bool redisp = true ;
	// Mover la cámara activa por la escena
	switch ( tecla )
	{      
		case 'Q' :
			exit( 0 );
			break ;		
			// Avanzar
		case 'W' :
			eye[2] -= 0.2;
			at[2] -= 0.2;
			break ;
			// Retroceder.
		case 'S' :
			eye[2] += 0.2;
			at[2] += 0.2;
			break ;
			// Desplazamiento a la izquierda.
		case 'D' :
			eye[0] += 0.2;
			at[0] += 0.2;
			break ;
			// Desplazamiento a la derecha.
		case 'A' :
			eye[0] -= 0.2;
			at[0] -= 0.2;
			break ;
			// Reiniciar posicion.  
		case 'R' :
			eye[0] = 0 ;
			at[0] = 0 ;
			camara_angulo_x = 0.0 ;
			camara_angulo_y = 0.0 ;
			break ;             
		default:
			redisp = false ;
			break ;
	}   
	// si se ha cambiado algo, forzar evento de redibujado
	return redisp ;
}



bool P5_FGE_TeclaEspecial( int tecla, int x_raton, int y_raton )
{
	bool redisp = true ;   
	switch ( toupper(tecla) )
	{	
		case GLUT_KEY_F1: // vista de frente y alzado
			camara_angulo_x=0;
			camara_angulo_y=0;
			break;	
		case GLUT_KEY_F2:
			P5_fijarProyeccion(); 
			camara_angulo_x=90;
			camara_angulo_y=0;
			break;
		case GLUT_KEY_F3:
			P5_fijarProyeccion(); 
			camara_angulo_x=0;
			camara_angulo_y=90;
			break;
		default:
			redisp = false ;
			break ;
	}
	return redisp ;
}

void P5_FGE_RatonMovido( int x, int y ) 
{
	if ( estadoRaton[1]==1 ){
		if(x>xr){
			camara_angulo_y++;
		}
		if(x<xr){
			camara_angulo_y--;
		}
		if(y>yr){
			camara_angulo_x++;
		}
		if(y<yr){
			camara_angulo_x--;
		}
	}if ( estadoRaton[0]==1 ){ }
	xr = x;
	yr = y;
}

void P5_FGE_RatonClick( int boton, int estado, int x, int y ) 
{
	if ( boton == GLUT_RIGHT_BUTTON )
	{
		if ( estado == GLUT_DOWN )
		{
			estadoRaton[1] = 1 ;
			xr = x;
			yr = y;
		}
		else
		{
			estadoRaton[1] = 0 ;
		}
	}
	// 	if ( boton == GLUT_LEFT_BUTTON )
	// 	{
	// 		if ( estado == GLUT_DOWN )
	// 		{
	// 			estadoRaton[0] = 1 ;
	// 			xr = x;
	// 			yr = y;            
	// 			pick( x, y );                         
	// 		}
	// 		else
	// 		{
	// 			estadoRaton[0] = 0 ;
	// 			id = 0;
	// 		}
	// 	}
}