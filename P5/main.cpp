// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Carlos Ureña Almagro
// **
// ** Función 'main', inicialización y gestores de eventos
// *********************************************************************

// includes de C/C++

#include <cctype>   // toupper
#include <string>   // std::string
#include <iostream> // std::cout
#include "stdio.h"
#include "stdlib.h"
#include <fstream>  // ifstream
#include <cmath>    // fabs
#include <vector>


// includes de OpenGL/glut

#include <GL/glut.h>  // freeglut standard (incluye cabecera opengl)
#include <GL/freeglut_ext.h> // extensiones freeglut (glutSolidCylinder)

// includes de archivos en el directorio de trabajo (de las prácticas)
#include "file_ply_stl.hpp"
#include "error-ogl.hpp"
#include "practica1.hpp"
#include "practica2.hpp"
#include "practica3.hpp"
#include "practica5.hpp"


using namespace std;


// *********************************************************************
// **
// ** Variables globales
// ** (se inicializan en las funciones de inicialización)
// **
// *********************************************************************

int num_cop, mov = 0 ;
char modo;
int selprac = 5 ;

// variables que definen la posicion de la camara en coordenadas polares

float 
camara_angulo_x ,   // angulo de rotación entorno al eje X
camara_angulo_y ;   // angulo de rotación entorno al eje Y

// ---------------------------------------------------------------------
// variables que definen el view-frustum (zona visible del mundo)

float 
frustum_factor_escala ,  // factor de escala homogeneo que se aplica al frustum (sirve para alejar o acercar)
frustum_ancho ,          // ancho, en coordenadas del mundo
frustum_alto ,
frustum_dis_del ,        // distancia al plano de recorte delantero (near)
frustum_dis_tra ,        // distancia al plano de recorte trasero (far)
eye[] = { 0, 0, 10 } ,
at[] = { 0, 0, 0 } ;

// ---------------------------------------------------------------------
// variables que determinan la posicion y tamaño inicial de la ventana 
// (el tamaño se actualiza al cambiar el tamaño durante la ejecución)

int 
ventana_pos_x  ,  // posicion (X) inicial de la ventana, en pixels 
ventana_pos_y  ,  // posicion (Y) inicial de la ventana, en pixels
ventana_tam_x  ,  // ancho inicial y actual de la ventana, en pixels
ventana_tam_y  ;  // alto inicial actual de la ventana, en pixels



// *********************************************************************
// **
// ** Funciones auxiliares:
// **
// *********************************************************************




// Inicializa la matriz GL_MODELVIEW 
void FijarVista()
{
	P5_fijarVista() ;	
}

// Fija la matriz GL_PROJECTION
void P5_FijarProyeccion( )
{

	// Resultado de dividir el numero de filas de pixels en el viewport, por el número de columnas. 
	P5_fijarProyeccion() ;
}

// Para conmutar entre prácticas

void Conmutar()
{
	if( selprac == 5 )
		selprac = 1 ;
	else
		selprac++ ;
}
void FijarProyeccion()
{
   const GLfloat ratioYX = GLfloat( ventana_tam_y )/GLfloat( ventana_tam_x );
   
   CError();
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // (3) proyectar en el plano de visión
   glFrustum
   (  -frustum_ancho,
      +frustum_ancho,
      -frustum_ancho*ratioYX,
      +frustum_ancho*ratioYX,
      +frustum_dis_del,
      +frustum_dis_tra
   );
   

   // (2) situar el origen (0,0,0), en mitad del view frustum 
   //     (en la rama negativa del eje Z)
   glTranslatef( 0.0,0.0,-0.5*(frustum_dis_del+frustum_dis_tra));
   
    // (1) aplicar factor de escala
   glScalef( frustum_factor_escala, frustum_factor_escala,  frustum_factor_escala );
   
   CError();
}


// ---------------------------------------------------------------------
// fijar viewport y proyección (viewport ocupa toda la ventana)

void FijarViewportProyeccion()
{
   glViewport( 0, 0, ventana_tam_x, ventana_tam_y );
   if(selprac != 5){P5_FijarProyeccion() ; }
   else FijarProyeccion();
}

// Función gestora del evento de click de ratón
void FGE_RatonClick( int boton, int estado, int x, int y )
{
	if( selprac == 5)
	{
		P5_FGE_RatonClick( boton, estado, x, y ) ;
		//glutPostRedisplay() ;
	}
}

// Función gestora del evento de movimiento del ratón
void FGE_RatonMovido( int x, int y )
{
	if( selprac == 5 )
	{
		P5_FGE_RatonMovido( x, y ) ;
		glutPostRedisplay() ;		
	}
}


// ---------------------------------------------------------------------
// fija la transformación de vista (posiciona la camara)

void FijarCamara()
{
	CError();
	if( selprac == 5 )
	{
		FijarVista() ;   
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();	   
		
		glRotatef(camara_angulo_x,1,0,0);
		glRotatef(camara_angulo_y,0,1,0);
	}
	
	
	
	//?
	//glScalef( factor_escala, factor_escala, factor_escala ); 
	CError();
}

// ---------------------------------------------------------------------
// dibuja los ejes utilizando la primitiva grafica de lineas

void DibujarEjes()
{
   const float long_ejes = 30.0 ;
   
   // establecer modo de dibujo a lineas (podría estar en puntos)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   
   // dibujar tres segmentos
   glBegin(GL_LINES);
      // eje X, color rojo
      glColor3f( 1.0, 0.0, 0.0 );
      glVertex3f( -long_ejes, 0.0, 0.0 );
      glVertex3f( +long_ejes, 0.0, 0.0 );
      // eje Y, color verde
      glColor3f( 0.0, 1.0, 0.0 );
      glVertex3f( 0.0, -long_ejes, 0.0 );
      glVertex3f( 0.0, +long_ejes, 0.0 );
      // eje Z, color azul
      glColor3f( 0.0, 0.0, 1.0 );
      glVertex3f( 0.0, 0.0, -long_ejes );
      glVertex3f( 0.0, 0.0, +long_ejes );
   glEnd();
   
   // bola en el origen, negra
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glColor3f(0.0,0.0,0.0);
   glutSolidSphere(0.01,8,8);
   
}

// ---------------------------------------------------------------------
// asigna el color de fondo actual a todos los pixels de la ventana

void LimpiarVentana()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


// ---------------------------------------------------------------------
// dibuja los objetos de la escena

void DibujarObjetos()
{
   switch(selprac){
      case 1:
        P1_DibujarObjetos(modo); // definido en 'practica1.hpp'
        break;
      case 2:
        P2_DibujarObjetos(modo); // definido en 'practica2.hpp'
        break;
      case 3:
         P3_DibujarObjetos(modo); // definido en 'practica3.hpp'
         break;
	  case 5:
		  P5_DibujarObjetos(modo); // definido en 'practica5.hpp'
		  break;
       default:
         P5_DibujarObjetos(modo); // definido en 'practica5.hpp'
         break;
   }
}

// *********************************************************************
// **
// ** Funciones gestoras de eventos
// **
// *********************************************************************

// F.G. del evento de redibujado (se produce cuando hay que volver a 
// dibujar la ventana, tipicamente tras producirse otros eventos)

void FGE_Redibujado()
{
	using namespace std ;
	//cout << "redibujado......" << endl << flush ;
	FijarViewportProyeccion() ; // necesario pues la escala puede cambiar
	FijarCamara();
	LimpiarVentana();
	DibujarEjes() ;
	DibujarObjetos();
	glutSwapBuffers();
}

// ---------------------------------------------------------------------
// F.G. del evento de cambio de tamaño de la ventana

void FGE_CambioTamano( int nuevoAncho, int nuevoAlto )
{
   // guardar nuevo tamaño de la ventana
   ventana_tam_x  = nuevoAncho;
   ventana_tam_y  = nuevoAlto ;
   
   // reestablecer frustum, viewport y proyección
   FijarViewportProyeccion();
   
   // forzar un nuevo evento de redibujado, para actualizar ventana
   glutPostRedisplay();
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla normal
//
// parámetros:
//       tecla: carácter corresondiente a la tecla (minúscula)
//       x_raton, y_raton : posición del ratón al pulsar

void FGE_PulsarTeclaNormal( unsigned char tecla, int x_raton, int y_raton )
{
   bool redisp = true ;
	switch (tecla)
	{
		case 'Q' :
			exit( 0 );
			break ;
		case '+' :
			frustum_factor_escala *= 1.05;
			break;
		case '-' :
			frustum_factor_escala /= 1.05;
			break;
		// Visualizar en modo 'Ajedrez'	
		case '1':
			modo = '1' ;
			break;
		// Visualizar en modo 'líneas/aristas'
		case '2':
			modo = '2' ;
			break;
		// Visualizar en modo 'Sólido'
		case '3':
			modo = '3' ;
			break;
		// Visualizar en modo 'Puntos'
		case '4':
			modo = '4' ;
			break;
		// Cambio en directo al estilo 'Normales'
		case '5':
			modo = '5' ;
			break;
		case 32:
			Conmutar() ;
			break ; 
		default:	   
			if(selprac==3) redisp = P3_FGE_TeclaNormal(tecla,x_raton,y_raton,modo);
			if( selprac == 5 ){redisp = P5_FGE_TeclaNormal( tecla, x_raton, y_raton  ) ;}  
			break ;
	}  

   
   // si se ha cambiado algo, forzar evento de redibujado
   if ( redisp)
      glutPostRedisplay() ;
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla especial
//
// parámetros:
//       tecla: código GLUT de la tecla pulsada
//       x_raton, y_raton : posición del ratón al pulsar


void FGE_PulsarTeclaEspecial( int tecla, int x_raton, int y_raton )
{
   bool redisp = true ;
   const float da = 5.0 ; // incremento en grados de ángulos de camara   
   switch ( tecla )
   {
      case GLUT_KEY_LEFT:
         camara_angulo_y = camara_angulo_y - da ;
         break;
      case GLUT_KEY_RIGHT:
         camara_angulo_y = camara_angulo_y + da ;
         break;
      case GLUT_KEY_UP:
         camara_angulo_x = camara_angulo_x - da ;
         break;
      case GLUT_KEY_DOWN:
         camara_angulo_x = camara_angulo_x + da ;
         break;
      case GLUT_KEY_PAGE_UP:
         frustum_factor_escala *= 1.05;
         break;
      case GLUT_KEY_PAGE_DOWN:
         frustum_factor_escala /= 1.05;
         break;
      default:
         redisp = false ;
         break ;
   }
   using namespace std ;

   // si se ha cambiado algo, forzar evento de redibujado
   if ( redisp )
      glutPostRedisplay();
}

// *********************************************************************
// **
// ** Funciones de inicialización
// **
// *********************************************************************

// inicialización de GLUT: creación de la ventana, designar FGEs

void Inicializa_GLUT( int argc, char * argv[] )
{
	
	// inicializa variables globales usadas en esta función (y otras)
	ventana_pos_x  = 50 ;
	ventana_pos_y  = 50  ;
	ventana_tam_x  = 1024 ;  // ancho inicial y actual de la ventana, en pixels
	ventana_tam_y  = 800 ;  // alto inicial actual de la ventana, en pixels
	
	// inicializa glut:
	glutInit( &argc, argv );
	
	// establece posicion inicial de la ventana:
	glutInitWindowPosition( ventana_pos_x, ventana_pos_y );
	
	// establece tamaño inicial de la ventana:
	glutInitWindowSize( ventana_tam_x, ventana_tam_y );
	
	// establece atributos o tipo de ventana:
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	
	// crea y visualiza una ventana:
	glutCreateWindow("Practicas IG (14-15)");
	
	// establece función gestora del evento de redibujado:
	glutDisplayFunc( FGE_Redibujado );
	
	// establece función gestora del evento de cambio de tamaño:
	glutReshapeFunc( FGE_CambioTamano );
	
	// establece función gestora del evento de pulsación de tecla normal:
	glutKeyboardFunc( FGE_PulsarTeclaNormal );
	
	// establece función gestora del evento de pulsación de tecla especial:
	glutSpecialFunc( FGE_PulsarTeclaEspecial );
	
	if(selprac == 5){
		glutMouseFunc( FGE_RatonClick);
		glutMotionFunc( FGE_RatonMovido );
	}
}

// ---------------------------------------------------------------------
// inicialización de OpenGL

void Inicializa_OpenGL( )
{
   // borrar posibles errores anteriores
   CError();
   
   // habilitar test de comparación de profundidades para 3D (y 2D)
   // es necesario, no está habilitado por defecto:
   // https://www.opengl.org/wiki/Depth_Buffer
   glEnable( GL_DEPTH_TEST );
   
   // establecer color de fondo: (1,1,1) (blanco)
   glClearColor( 0.7, 0.7, 0.7, 1.0 ) ;
   
   // establecer color inicial para todas las primitivas
   glColor3f( 0.7, 0.2, 0.4 ) ;
   
   // establecer ancho de línea
   glLineWidth( 1.0 );
   
   // establecer tamaño de los puntos
   glPointSize( 2.0 );
   
   // establecer modo de visualización de prim.
   // (las tres posibilidades son: GL_POINT, GL_LINE, GL_FILL)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   
   // inicializar parámetros del frustum
   frustum_dis_del         = 0.1 ;
   frustum_dis_tra         = 40.0;
   frustum_ancho           = 0.5*frustum_dis_del ;
   frustum_factor_escala   = 0.2 ;
   
   // inicializar parámetros de la cámara
   camara_angulo_x = 0.0 ;
   camara_angulo_y = 0.0 ;

   // establecer la cámara, la proyección y el viewport
   FijarViewportProyeccion() ;
   FijarCamara() ;
   
   // ya está
   CError();
}

// ---------------------------------------------------------------------
// Código de inicialización (llama a los dos anteriores, entre otros)

void Inicializar( int argc, char *argv[] )
{
   // glut (crea la ventana)
   Inicializa_GLUT( argc, argv ) ;
   
   // opengl: define proyección y atributos iniciales
   Inicializa_OpenGL() ;
   
   // inicializar práctica 1: carga el ply
   P1_Inicializar( argc, argv ) ;

   // inicializar práctica 2: carga el perfil
   P2_Inicializar( argc, argv ) ;

   // inicializar práctica 3
   P3_Inicializar() ;
   
   P5_Inicializar(argc, argv );
}

// *********************************************************************
// **
// ** Función principal
// **
// *********************************************************************


int main( int argc, char *argv[] )
{

     // incializar el programa
   	Inicializar( argc, argv ) ;
	
	// llamar al bucle de gestión de eventos de glut, tiene el 
   	// control hasta el final de la aplicación
   	glutMainLoop() ;
	
   	// ya está
   	return 0;
}
