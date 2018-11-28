// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.h
// -- declaraciones de clase para el objeto jerárquico de la práctica 3
//
// #############################################################################

#ifndef GRAFOPARAM_H_INCLUDED
#define GRAFOPARAM_H_INCLUDED

#include "malla.h" // añadir .h de cualquier objetos malla indexada usados....

constexpr int num_parametros = 4; // número de parámetros o grados de libertad
                                  // de este modelo


class GrafoParam
{
    public:
      // crea mallas indexadas (nodos terminales del grafo)
      GrafoParam();

      // función principal de visualización
      void draw(const int p_modo_vis, const bool p_usar_diferido);

      // actualizar valor efectivo de un parámetro (a partir de su valor no acotado)
      void actualizarValorEfe(const unsigned iparam, const float valor_na);

      // devuelve el número de parámetros
      unsigned numParametros() { return num_parametros; }

    private:
      // métodos de dibujo de subgrafos
      void dibujo_completo();
      void centro();
      void lateral_izda();
      void panel_izda();
      void lateral_dcha();
      void panel_dcha();
      void base();
      void planeta();
      
      //void examen();

      // objetos tipo malla indexada (nodos terminales)
      Cubo      *cubo     = nullptr;
      Cubo      *panel    = nullptr;
      Cilindro  *cilindro = nullptr;
      Esfera    *esfera   = nullptr;
      Cono      *cono     = nullptr;

      // parámetros de la llamada actual (o última) a 'draw'
      int modo_vis;
      bool usar_diferido; // modo de envío (true -> diferido, false -> inmediato)

      // valores efectivos de los parámetros (angulos, distancias, factores de
      // escala, etc.....) calculados a partir de los valores no acotados

      float ag_rotacion_1, // ángulo en grados de rotación (1)
          ag_rotacion_2, // ángulo en grados de rotación (2)
          altura_2,      // altura de la segunda columna
          ag_rotacion_3;

};

#endif
