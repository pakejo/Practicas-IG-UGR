#ifndef _LUZ_H
#define _LUZ_H

#include "aux.h"
#include <vector>

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
    GLdouble translate[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1};

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

#endif