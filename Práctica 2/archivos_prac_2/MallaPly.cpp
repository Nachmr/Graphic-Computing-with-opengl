#include <GL/glut.h>

#include "MallaPly.h"
#include "error-ogl.hpp"
#include "math.h"

using namespace std;


//----------------------------------------------------------------------
//  Estructura de datos que almacena la malla formada por los datos del ply


MallaPly::MallaPly(const vector<float> ver, const vector<int> tri){
  mesh_num_verts=ver.size()/3;
  mesh_num_tri=tri.size()/3;
        cout << "Llegaaj, num_car:"  << mesh_num_tri << endl;

  //Rellenamos el vector de vertices
  for(unsigned i=0; i<ver.size(); i=3+i){
    Tupla3f vi(ver.at(i+0),ver.at(i+1),ver.at(i+2));
    mesh_verts.push_back(vi);
  }
  
  //Rellenamos el vector de indices
  for(unsigned i=0; i<tri.size(); i=3+i){
    Tupla3i indexi = {tri.at(i+0),tri.at(i+1),tri.at(i+2)};
    mesh_tri.push_back(indexi);   
  }
  
  //Cálculo de las normales de las caras
  for(unsigned i=0; i<mesh_num_tri; ++i){//cada normal i corresponde a la cara i
    Tupla3f a(mesh_verts.at(mesh_tri.at(i).idx[0]).coo);
    Tupla3f b(mesh_verts.at(mesh_tri.at(i).idx[1]).coo);
    Tupla3f c(mesh_verts.at(mesh_tri.at(i).idx[2]).coo);
    Tupla3f ab(b - a);
    Tupla3f bc(c - b);
    Tupla3f n(ab * bc);
    mesh_norm.push_back(normalized(n));
  }
  
  //Calculo de las normales de vértices
  vert_norm = vector<Tupla3f>(mesh_num_verts, Tupla3f(0,0,0));
  
  for(unsigned i = 0; i<mesh_num_tri; ++i){
    vert_norm.at(mesh_tri.at(i).idx[0]) = vert_norm.at(mesh_tri.at(i).idx[0]) + mesh_norm.at(i);
    vert_norm.at(mesh_tri.at(i).idx[1]) = vert_norm.at(mesh_tri.at(i).idx[1]) + mesh_norm.at(i);
    vert_norm.at(mesh_tri.at(i).idx[2]) = vert_norm.at(mesh_tri.at(i).idx[2]) + mesh_norm.at(i);
  }
  
  //Normalizamos los vectores
  for(unsigned i = 0; i<mesh_num_verts; ++i){
    vert_norm.at(i) = normalized(vert_norm.at(i));
  }

}
/*****************************************************
//Funciones para dibujar el PLY en modo glBegin/glEnd
*****************************************************/

void MallaPly::DibujarBE() {
  glBegin( GL_TRIANGLES) ;
    for( unsigned i = 0 ; i < mesh_num_tri ; ++i){
      
      //colorear con normales de caras
      //glColor3f(fabs(mesh_norm.at(i).coo[0]),fabs(mesh_norm.at(i).coo[1]),fabs(mesh_norm.at(i).coo[2]));
      //clorear con normales de vertices
               cout << "contador bucle glbegin:" << i << endl;

      GLuint ind_ver = mesh_tri[i].idx[0];
      glColor3f(fabs(vert_norm.at(ind_ver).coo[0]), fabs(vert_norm.at(ind_ver).coo[1]),fabs(vert_norm.at(ind_ver).coo[2]));
      glVertex3fv(&(mesh_verts[ind_ver].coo[0]));
      
      ind_ver = mesh_tri[i].idx[1];
      glColor3f(fabs(vert_norm.at(ind_ver).coo[0]), fabs(vert_norm.at(ind_ver).coo[1]),fabs(vert_norm.at(ind_ver).coo[2]));
      glVertex3fv(&(mesh_verts[ind_ver].coo[0]));
      
      ind_ver = mesh_tri[i].idx[2];
      glColor3f(fabs(vert_norm.at(ind_ver).coo[0]), fabs(vert_norm.at(ind_ver).coo[1]),fabs(vert_norm.at(ind_ver).coo[2]));
      glVertex3fv(&(mesh_verts[ind_ver].coo[0]));	
    }
  glEnd();
  glShadeModel(GL_SMOOTH);
}

void MallaPly::DibujarBE_vertices() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  DibujarBE();
}

void MallaPly::DibujarBE_alambre() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  DibujarBE();
}

void MallaPly::DibujarBE_solido() {
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_LINE);
  DibujarBE();
}

void MallaPly::DibujarBE_ajedrez() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin( GL_TRIANGLES) ;
  unsigned i; //tengo que sacar la declaracion del bucle porque si no gcc no me deja operar con i dentro.
    for( i = 0 ; i < mesh_num_tri ; ++i){
      if(i%2==0) glColor3f(0.5,0,1);
      else glColor3f(0,1,0.5);
      
      GLuint ind_ver = mesh_tri[i].idx[0];
      glVertex3fv(&(mesh_verts[ind_ver].coo[0]));
      ind_ver = mesh_tri[i].idx[1];
      glVertex3fv(&(mesh_verts[ind_ver].coo[0]));
      ind_ver = mesh_tri[i].idx[2];
      glVertex3fv(&(mesh_verts[ind_ver].coo[0]));	
    }
  glEnd();
}

/**************************************************
 **Funciones para dibujar en modo glDrawElements **
 **************************************************/

void MallaPly::DibujarDE_vertices() {
  CError();
  glColor3f(0.5,0,0.2);
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, &(mesh_verts.at(0).coo[0] ));
  glDrawArrays( GL_POINTS, 0, mesh_num_verts) ;
}

void MallaPly::DibujarDE_alambre() {
  CError();
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, &(mesh_verts.at(0).coo ));
  CError();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements( GL_TRIANGLES, mesh_num_tri*3 , GL_UNSIGNED_INT, &(mesh_tri.at(0).idx) ) ;
}

void MallaPly::DibujarDE_solido() {
  CError();
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, &(mesh_verts.at(0).coo ));
  CError();
  glDrawElements( GL_TRIANGLES, mesh_num_tri*3 , GL_UNSIGNED_INT, &(mesh_tri.at(0).idx) ) ;
}

void MallaPly::DibujarDE_ajedrez() {
  CError();
  Tupla3f color1(0.5,0,1);
  Tupla3f color2(0,1,0.5);
  vector<Tupla3i> caras_p;
  vector<Tupla3i> caras_i;
  for(unsigned i=0; i<mesh_num_tri; ++i){
    if(i%2==0) caras_p.push_back(mesh_tri.at(i));
    else caras_i.push_back(mesh_tri.at(i));
  }
  
  glEnableClientState( GL_VERTEX_ARRAY );
  CError();
  glColor3f(0.5,0,1);
  glVertexPointer( 3, GL_FLOAT, 0, &(mesh_verts[0].coo ));
  glDrawElements( GL_TRIANGLES, caras_p.size()*3 , GL_UNSIGNED_INT, &(caras_p[0].idx) ) ;
  glColor3f(0.7,1,0.3);
  glVertexPointer( 3, GL_FLOAT, 0, &(mesh_verts[0].coo ));
  glDrawElements( GL_TRIANGLES, caras_i.size()*3 , GL_UNSIGNED_INT, &(caras_i[0].idx) ) ;
}