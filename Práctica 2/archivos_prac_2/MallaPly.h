#ifndef MALLAPLY_H
#define MALLAPLY_h

#include "tuplas.hpp"   // Tupla3f, Tupla4f, Tupla3i
#include <iostream>
#include <vector>

using namespace std;


class MallaPly{
  private:
    unsigned mesh_num_verts;	//Numero total de vértices
    unsigned mesh_num_tri;	//Numero total de caras
    vector<Tupla3f> mesh_verts;	//Lista de vértices (x,y,z)
    vector<Tupla3f> vert_norm;	//Lista de normales a vértices (x,y,z)
    vector<Tupla3i> mesh_tri;	//Lista de caras triangulares (v1,v2,v3)
    vector<Tupla3f> mesh_norm;	//Lista de normales a las caras (x,y,z)
    void DibujarBE();		//Metodo para dibujar los vertices en modo glBegin/glEnd
  public:
    MallaPly( const vector<float> ver, const vector<int> tri);
    inline unsigned getNumTri(){return mesh_num_tri;};
    inline unsigned getNumVerts(){return mesh_num_verts;};
    //Métodos para dibujar en modo glBegin/glEnd
    void DibujarBE_vertices();
    void DibujarBE_alambre();
    void DibujarBE_solido();
    void DibujarBE_ajedrez();
    //Métodos para dibujar en modo glDrawElements
    void DibujarDE_vertices();
    void DibujarDE_alambre();
    void DibujarDE_solido();
    void DibujarDE_ajedrez();
    void PintarNorVer();
}; 

#endif