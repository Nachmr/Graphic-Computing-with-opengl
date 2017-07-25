// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 2  (implementación)
// **
// ** Anotaciones...
// **
// ** Numero de vertices del cuerpo de revolucion: M*N+2
// ** M: vertices del perfil inicial
// ** N: numero de rotaciones
// ** 
// ** Para caras, usar módulos con los indices. i+M para vertices a igual altura(i+M*j)
// ** Vertices imodM=0 enganchan con tapa superior
// ** Vertices imodM=M-1 enganchan con tapa inferior
// **
// ** 
// **
// *********************************************************************


#include <GL/glut.h>

#include "error-ogl.hpp"
#include "practica2.hpp"
#include <iostream>
#include <cstdlib>
#include <math.h>

//includes para lectura de ply

#include <vector>
#include <algorithm>
#include "file_ply_stl.hpp"

//include para mi clase MallaPly, tambien incluye el tuplas.hpp
#include "MallaPly.h"


using namespace std;

const double PI=3.141592;//65359;

vector<float> verts_plyP2;
vector<int> caras_plyP2;

//puntero global a la malla
MallaPly * meshP2;
//----------------------------------------------------------------------
// Funciones auxiliares
//----------------------------------------------------------------------

/*Esta funcion simple analiza si los puntos están en orden ascendente o descendente
* para construir correctamente las caras han de estar de mayor valor de Y a menor, 
* asi  que si no lo está, se invierten los vertices para dejarlo correctamente orientado*/

void AnalizaPly(vector<float> & vec){
  if(vec.at(1) < vec.at(vec.size()-2)){
    cout << "Polilinea invertida, volteando..." << endl;
    reverse(vec.begin(),vec.end());
  }
}

void CrearCuerpo( vector<float> & dest, vector<float> & orig, int n, vector<int> & caras ){
  //Inicializamos el vector destino con los puntos del perfil original
  //dest = orig;
  dest.clear();
  //Calculo del resto
  double rad;
  for(unsigned i = 0; i < n; ++i){
    for(unsigned j = 0; j< orig.size(); j=j+3){
	rad = 2*PI*i/n;
	float cosen = cos(rad);
	float sen = sin(rad);
	dest.push_back(orig.at(j)*cosen + orig.at(j+2)*sen);
	dest.push_back(orig.at(j+1));
	dest.push_back(orig.at(j+2)*cosen-orig.at(j)*sen);
    }
  }
  
  /* Ahora a crear las caras*/
  unsigned M = orig.size()/3; //num. vertices del perfil original
  unsigned K = dest.size()/3; // vertices totales incl. tapas
  
  for(unsigned i = 0; i<K; ++i){//cada i es un vertice del cuerpo
    if(i%M == 0){//si toca con la tapa superior...
      caras.push_back(i); //él mismo en el siguiente triangulo
      caras.push_back(i+1);// el siguiente del perfil
      caras.push_back((i+M)%K); // de nuevo el contiguo del perfil siguiente
    }
    else if(i%M == (M-1)){//si toca la tapa inferior...
      caras.push_back(i);
      caras.push_back((i+M)%K);//perfil siguiente
      caras.push_back((i+M)%K-1);//perfil siguiente
    }
    else{
      caras.push_back(i);
      caras.push_back((i+M)%K);//perfil siguiente
      caras.push_back((i+M)%K-1);//perfil siguiente
      caras.push_back(i); //él mismo en el siguiente triangulo
      caras.push_back(i+1);// el siguiente del perfil
      caras.push_back((i+M)%K); // de nuevo el contiguo del perfil siguiente
    }
  }
}


  /* Para crear los vertices de las tapas, usamos el primer y ultimo puntos.     *
   * El de la tapa superior tendrá la coordenada Y del primer punto, y la X y Z  *
   * serán 0 ya que está contenido en el eje Y. Igual pasa con el de la tapa de  *
   * abajo pero usando el último punto.					   */
  
void CrearTapas( vector<float> & dest, vector<float> orig, vector<int> & caras,  Tupla3f vert_sup, Tupla3f vert_inf){
  dest.push_back(vert_sup.coo[0]);//coord X de arriba
  dest.push_back(vert_sup.coo[1]); //coord Y tapa de arriba
  dest.push_back(vert_sup.coo[2]);//coord Z de arriba
  dest.push_back(vert_inf.coo[0]);//coord X de abajo
  dest.push_back(vert_inf.coo[1]);//coord Y tapa de abajo
  dest.push_back(vert_inf.coo[2]);//coord Z de abajo
  
  unsigned M = orig.size()/3;
  unsigned K = dest.size()/3; // actualizo K
  
  //Y se crean las correspondientes caras
  for (unsigned i = 0; i < K-M-2; i = i+M){
    caras.push_back(i);
    caras.push_back(i+M);
    caras.push_back(K-2);
  }
  
  //la ultima tiene tratamiento diferenciado
  caras.push_back(K-2-M); //Primer vertice de ULTIMO perfil
  caras.push_back(0);	//Primer vertice de PRIMER perfil
  caras.push_back(K-2);	//Vert. tapa superior
  
  for (unsigned i = M-1; i < K-M-2; i = i+M){
    caras.push_back(i);
    caras.push_back(K-1);
    caras.push_back(i+M);
  }
  
  //la ultima cara tiene tratamiento diferente
  caras.push_back(K-3);//ULTIMO vertice de ULTIMO perfil
  caras.push_back(K-1);//Vert. tapa inferior
  caras.push_back(M-1);//ULTIMO vertice de PRIMER perfil
  
  cout << "Cuerpo de revolución generado\n";

}

void CrearRevolucion( vector<float> & dest, vector<float> & orig, int n, vector<int> & caras){
  if(orig.at(0)==0 && orig.at(2)==0){//El vertice superior esta en el ply
    cout << "SUPERIOR  NA JNFNJ OSO\n";
    if(orig.at(orig.size()-3) == 0 && orig.at(orig.size()-1) == 0){//el inferior tambien
      cout << "Este PLY tiene dos vertices en el eje Y\n";
      //Se extraen los vertices
      Tupla3f vert_sup(orig.at(0),orig.at(1),orig.at(2));
      Tupla3f vert_inf(orig.at(orig.size()-3),orig.at(orig.size()-2),orig.at(orig.size()-1));
      //Se eliminan del vector origen
      orig.erase(orig.begin(),orig.begin()+3);
      for(unsigned i=0;i<3;++i) orig.pop_back();
      
      CrearCuerpo(dest,orig,n,caras);
      CrearTapas(dest, orig, caras, vert_sup, vert_inf);
    }
    else{
      cout << "Este PLY solo tiene el vertice superior en el eje Y\n";
      Tupla3f vert_sup(orig.at(0),orig.at(1),orig.at(2));
      Tupla3f vert_inf(0,orig.at(orig.size()-2),0);
      //se elimina solo el superior
      orig.erase(orig.begin(),orig.begin()+3);
      CrearCuerpo(dest,orig,n,caras);
      CrearTapas(dest,orig, caras,vert_sup,vert_inf);
    }
  }
  else{
    cout << "INFERIOR ANGPOUANS\n";
    if(orig.at(orig.size()-3) == 0 && orig.at(orig.size()-1) == 0){//el inferior si esta
    cout << "Este PLY solo tiene el vertice inferior en el eje Y\n";
      Tupla3f vert_sup(0,orig.at(1),0);
      Tupla3f vert_inf(orig.at(orig.size()-3),orig.at(orig.size()-2),orig.at(orig.size()-1));
      //Se elimina solo el vertice inferior
      for(unsigned i=0;i<3;++i) orig.pop_back();
      CrearCuerpo(dest,orig,n,caras);
      CrearTapas(dest, orig, caras, vert_sup, vert_inf);
    }else{
      cout << "Este PLY no tiene vertices en el eje Y\n";
      //Se extraen los vertices
      Tupla3f vert_sup(0,orig.at(1),0);
      Tupla3f vert_inf(0,orig.at(orig.size()-2),0);

      //no se elimina nada
      CrearCuerpo(dest,orig,n,caras);
      CrearTapas(dest, orig, caras, vert_sup, vert_inf);
    }
  }
}

void P2_Inicializar( int argc, char *argv[] )
{
  //argv[1]: plyP1; argv[2]:plyp2; argv[3]: num. rotaciones.
  if(argc>1){
    int num_rot = atoi(argv[3]);
    if(num_rot < 3){
      cerr << "Error en numero de rotaciones: debe ser mayor de 2\nAbortando ejecucion...\n";
      exit(EXIT_FAILURE);
    }
    
    ply::read_vertices(argv[2],verts_plyP2);//leemos el ply
    AnalizaPly(verts_plyP2);
    vector<float> vertices_rev;
    vector<int> caras_rev;
    CrearRevolucion( vertices_rev, verts_plyP2, num_rot, caras_rev );
    
    meshP2 = new MallaPly(vertices_rev,caras_rev); //creamos la MallaPly con los datos leídos
     
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


void P2_DibujarObjetos(int modo_vis, int modo_dib) 
{
    switch(modo_vis){//Miramos el modo de visualización...
      case 1: //Modo puntos
	if(modo_dib==1) meshP2->DibujarBE_vertices();//glBegin/glEnd
	else meshP2->DibujarDE_vertices();		  //glDrawElements
	break;
      case 2://Modo alambre
	if(modo_dib==1) meshP2->DibujarBE_alambre();
	else meshP2->DibujarDE_alambre();	
	break;
      case 3://Modo solido
	if(modo_dib==1) meshP2->DibujarBE_solido();
	else meshP2->DibujarDE_solido();	
	break;
      case 4://Modo ajedrez
	if(modo_dib==1) meshP2->DibujarBE_ajedrez();
	else meshP2->DibujarDE_ajedrez();
	break;
      default:
	cout << "Ninguna opcion válida... Dibujando en modo puntos" << endl;
	meshP2->DibujarDE_vertices();
	break;
  }
/*
  glColor3f(0.5,0.5,0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  glBegin( GL_TRIANGLES) ;
    for( unsigned i = 0 ; i < vertices.size() ; i=i+3){
      glVertex3f(vertices.at(i),vertices.at(i+1),vertices.at(i+2));	
      }
  glEnd(); 
}*/
}