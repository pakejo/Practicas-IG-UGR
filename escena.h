#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "jerarquico.h"
#include "luz.h"
#include "camara.h"

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

    // Iluminacion
    bool activarAnimaciones;
    bool luz;
    bool encender_luz;

    //  Camaras
    int camaraActiva = 0;
    float xant;
    float yant;
    float zoomAnt;

    bool seMueveCamara;
    bool seHaceZoom;

    std::vector<Camara> camaras;

    //Cambio por seleccion
    std::vector<int> vector_cambio;
    std::vector<Tupla3i> color_obj;


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
    Piramide *piramide = nullptr;

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

    //Camaras
    void dibujaSeleccionFalso();
    void dibujaSeleccion();
    void clickRaton(int boton, int estado, int x, int y);
    void ratonMovido(int x, int y);
    void pick_color(int x, int y);
};

#endif
