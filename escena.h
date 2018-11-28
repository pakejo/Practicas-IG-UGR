#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "jerarquico.h"


class Luz
{
  private:

    //Colores de la luz
    float 
    color_difuso[4],
    color_especular[4],
    color_ambiental[4];

    //Posicion de la luz
    float pos[4];

  public:

    Luz(float cdf[], float cef[], float caf[], float posf[]);

    //Activacion de la iluminacion
    void activar();

    //Desactivacion de la iluminacion
    void desactivar();
    
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

  bool activarAnimaciones;

  // Objetos de la escena
  Cubo *cubo = nullptr;
  Tetraedro *tetraedro = nullptr;
  ObjPLY *PLY = nullptr;
  ObjRevolucion *Rev = nullptr;
  Cono *cono = nullptr;
  Cilindro *cilindro = nullptr;
  Esfera *esfera = nullptr;
  ObjJerarquico *jerarquico = nullptr;
  Piramide *piramide = nullptr;

  Luz *alfa = nullptr;
  Luz *beta = nullptr;

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
