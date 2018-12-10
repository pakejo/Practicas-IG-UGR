#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "jerarquico.h"

struct Foco
{
  //Colores de la luz
  float
      color_difuso[4],
      color_especular[4],
      color_ambiental[4];

  //Posicion de la luz
  float pos[4];
};

class Luz
{
private:
  //Vector con los datos de las luces activas
  std::vector<Foco> datos_luces;

  //Variables para saber la luz a usar
  std::vector<GLenum> luces = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

  //Angulo de rotacion de la luz
  float angulo_rotacion = 0.0;

  //Matriz identidad
  GLdouble translate[16] = {1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1};

public:
  Luz(float cdf[], float cef[], float caf[], float posf[]);

  //Activacion de la iluminacion
  void activar();

  //Desactivacion de la iluminacion
  void desactivar();

  //Añadir un nuevo foco
  void nueva_luz(float cdf[], float cef[], float caf[], float posf[]);

  //Incremento del angulo de rotacion
  void incrementa_angulo();
};

class Escena
{

private:
  Ejes ejes;

  // variables que definen la posicion de la camara en coordenadas polares
  GLfloat Observer_distance;
  GLfloat Observer_angle_x;
  GLfloat Observer_angle_y;

  // variables que controlan la ventana y la transformacion de perspectiva
  GLfloat Width, Height, Front_plane, Back_plane;

  void clear_window();
  void dibujar_objeto_actual();

  // Transformación de cámara
  void change_projection(const float ratio_xy);
  void change_observer();

  void conmutarAnimaciones();

  int objeto_actual = 0, // objeto actual (el que se visualiza)
      num_objetos = 0,   // número de objetos (actualizado al crear los objetos en el constructor)
      mode = 0;          // cambia el modo de visualizacion

  bool cambia_modo = false; //Indica modo GPU
  bool shade_model = true;

  bool activarAnimaciones;
  bool luz;

  // Objetos de la escena
  Cubo *cubo = nullptr;
  Tetraedro *tetraedro = nullptr;
  ObjPLY *PLY = nullptr;
  ObjRevolucion *Rev = nullptr;
  Cono *cono = nullptr;
  Cilindro *cilindro = nullptr;
  Esfera *esfera = nullptr;
  ObjJerarquico *jerarquico = nullptr;
  Cuadro *cuadro = nullptr;

  Luz *foco = nullptr;

public:
  Escena();
  void inicializar(int UI_window_width, int UI_window_height);
  void redimensionar(int newWidth, int newHeight);

  // Dibujar
  void dibujar();

  // Interacción con la escena
  bool teclaPulsada(unsigned char Tecla1, int x, int y);
  void teclaEspecial(int Tecla1, int x, int y);

  //Animaciones
  void mgeDesocupado();
};

#endif
