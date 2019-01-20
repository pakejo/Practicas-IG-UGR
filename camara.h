#ifndef _CAMARA_H
#define _CAMARA_H

#include "aux.h"

class Camara
{
  private:
    Tupla3f eye;  //Posicion donde colocar la camara
    Tupla3f at;   //Punto al que queremos que mire la camara
    Tupla3f up;   //Orientacion de la camara

    int tipo; //Ortogonal o Perspectiva
    float left, right, bottom, top, near, far;
    int ratio_xy;

    float observer_angle_x;   //Rotacion en el eje y
    float observer_angle_y;   //Rotacion en el eje x

    float xant; //Posicion del raton eje x
    float yant; //Posicion del raton eje y
    float zoomAnt;

    float Zoom;

    void rotarX(float angle);
    void rotarY(float angle);
    
  public:
    Camara(Tupla3f eye, int tipo);
    void mover(float x, float y);
    void zoom(float factor);

    void setObserver();
    void setProyection(const float ratio_xy);
};

#endif