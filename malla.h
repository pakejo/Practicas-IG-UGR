// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: ObjMallaIndexada.h
// -- declaraciones de clase ObjMallaIndexada (mallas indexadas) y derivados
//
// #############################################################################

#ifndef OBJETOS_H_INCLUDED
#define OBJETOS_H_INCLUDED

#ifdef MACOS
#define cimg_display 0
#endif

#include "aux.h"
#include <cmath>
#include "CImg.h"
using namespace cimg_library;

// *****************************************************************************
//
// Estructura para datos de material
//
// *****************************************************************************
struct Material
{
  std::vector<GLfloat> ref_difusa = {0.50754, 0.50754, 0.50754, 1.0},
                       ref_especular = {0.508273, 0.508273, 0.508273, 1.0},
                       ref_ambiental = {0.19225, 0.19225, 0.19225, 1.0};

  GLfloat brillo = 0.4;

  void activar();
};

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

class ObjMallaIndexada
{
public:
  // dibuja el objeto en modo inmediato
  void draw_ModoInmediato(int modo_vis);

  // dibuja el objeto en modo diferido (usando VBOs)
  void draw_ModoDiferido();

  // función que redibuja el objeto
  void draw(int modo_vis, bool GPU_mode);

  //funcion que dibuja el modo ajedrez
  void draw_ModoAjedrez();

  void activar_Material();

  void PreparaTextura();

protected:
  void calcular_normales();                 // calcula tabla de normales de vértices (práctica 3)
  void colorear();                          //Funcion que va llenando el vector de colorees en funcion del objeto y sus vertices
  void cargar_imagen(const char *fileName); //Funcion para cargar una imagen en memoria
  void calcular_coord_tex();

  std::vector<Tupla3f> vertices;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
  std::vector<Tupla3i> triangulos; // una terna de 3 enteros por cada cara o triángulo

  GLuint id_vbo_ver, id_vbo_tri, id_vbo_colores; // identificadores de vbo

  // completar: tabla de colores, tabla de normales de vértices
  std::vector<Tupla3f> colores;
  std::vector<Tupla3f> normales_vertices;
  std::vector<Tupla2f> coord_textura;

  // Datos del material
  Material material;

  // Datos para texturas
  std::vector<unsigned char> pixels;
  GLuint textura_id;
  int ancho, alto;
  std::vector<Tupla3f> verticesP;
  std::vector<Tupla3i> triangulosP;
};

// *****************************************************************************
//
// clases derivadas de ObjMallaIndexada (definen constructores específicos)
//
// *****************************************************************************

// *****************************************************************************
// Cubo con centro en el origen y lado unidad
// (tiene 9 vertices y 6 caras)

class Cubo : public ObjMallaIndexada
{
public:
  Cubo();
};

// *****************************************************************************
// Tetraedro con centro en el origen y lado unidad
// (tiene 4 vertices y 4 caras)
class Tetraedro : public ObjMallaIndexada
{
public:
  Tetraedro();
};

// *****************************************************************************
// objeto leído de un archivo PLY

class ObjPLY : public ObjMallaIndexada
{
public:
  ObjPLY(const std::string &nombre_archivo);
};

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

class ObjRevolucion : public ObjMallaIndexada
{
public:
  //ObjRevolucion(const float radio1, const float radio2);
  ObjRevolucion(const std::string &nombre_ply_perfil);

protected:
  void crearMalla(const std::vector<Tupla3f> perfil_original, const int num_instancias_perfil, std::vector<Tupla3f> &vertices, std::vector<Tupla3i> &triangulos);
};

// *****************************************************************************
//
// clases derivadas de ObjRevolucion (definen constructores específicos)
//
// *****************************************************************************

class Cilindro : public ObjRevolucion
{
public:
  Cilindro(const std::string &nombre_ply_perfil);
};

class Cono : public ObjRevolucion
{
public:
  Cono(const std::string &nombre_ply_perfil);
};

class Esfera : public ObjRevolucion
{
public:
  Esfera(const std::string &nombre_ply_perfil);
};

class Piramide : public ObjMallaIndexada
{
public:
  Piramide();
};

class Cuadro : public ObjMallaIndexada
{
private:
  void CuadroEsquina();

public:
  Cuadro();
};

#endif
