// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// **
// ** Práctica 3  (declaraciones públicas)
// *********************************************************************

#ifndef IG_PRACTICA3_HPP
#define IG_PRACTICA3_HPP

#include <GL/gl.h>
#include <vector>
#include <iostream>
#include "mallaply.h"
#include "file_ply_stl.hpp" //Funciones para leer archivo ply
#include <vector>


void P3_DibujarObjetos(char modo);
void P3_Inicializar() ;
bool P3_FGE_TeclaNormal( unsigned char tecla, int x_raton, int y_raton, char & P3_modo ) ;

#endif
