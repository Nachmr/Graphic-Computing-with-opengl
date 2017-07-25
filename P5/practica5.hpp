// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// **
// ** Práctica 3  (declaraciones públicas)
// ** Ignacio Martín Requena
// *********************************************************************

#ifndef IG_PRACTICA5_HPP
#define IG_PRACTICA5_HPP

#include <GL/gl.h>
#include <vector>
#include <iostream>

using namespace std;


void clear();
void P5_DibujarObjetos( char modo);
void P5_Inicializar( int argc, char *argv[] ) ;	
void P5_fijarProyeccion(  ) ;
void P5_fijarVista() ;	
void pick(unsigned int x, unsigned int y ) ;
void P5_Examinar( int x ) ;
void P5_ProcesarHits( GLint hits, GLuint buffer[] ) ;
bool P5_FGE_TeclaEspecial( int tecla, int x_raton, int y_raton ) ;
bool P5_FGE_TeclaNormal( unsigned char tecla, int x_raton, int y_raton ) ;
void P5_FGE_RatonMovido( int x, int y ) ;
void P5_FGE_RatonClick( int boton, int estado, int x, int y ) ;


#endif
