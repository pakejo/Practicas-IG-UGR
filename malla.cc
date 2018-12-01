#include "aux.h"
#include "ply_reader.h"
#include "malla.h"
#include "escena.h"

// *****************************************************************************
//
// Clase ObjMallaIndexada
//
// *****************************************************************************

// Visualización en modo inmediato con 'glDrawElements'

void ObjMallaIndexada::draw_ModoInmediato(int modo_vis)
{
  // visualizar la malla usando glDrawElements,
  // completar (práctica 1)
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

  if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
    glNormalPointer(GL_FLOAT, 0, normales_vertices.data());

  if (modo_vis == 3)
    this->draw_ModoAjedrez();
  else
    glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT, triangulos.data());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

void ObjMallaIndexada::draw_ModoAjedrez()
{
  std::vector<Tupla3i> triangulos1, triangulos2;
  std::vector<Tupla3f> color1, color2;

  for (int i = 0; i < triangulos.size(); i++)
  {
    if (i % 2 == 0)
      triangulos1.push_back(triangulos[i]);
    else
      triangulos2.push_back(triangulos[i]);
  }

  for (int i = 0; i < vertices.size(); i++)
  {
    color1.push_back({0.0, 1.0, 0.0});
    color2.push_back({0.0, 0.0, 1.0});
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, vertices.data());
  glColorPointer(3, GL_FLOAT, 0, color1.data());
  glDrawElements(GL_TRIANGLES, triangulos1.size() * 3, GL_UNSIGNED_INT, triangulos1.data());
  glColorPointer(3, GL_FLOAT, 0, color2.data());
  glDrawElements(GL_TRIANGLES, triangulos2.size() * 3, GL_UNSIGNED_INT, triangulos2.data());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}
// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)
GLuint CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram)
{
  GLuint id_vbo;
  // resultado: identificador de VBO
  glGenBuffers(1, &id_vbo);
  // crear nuevo VBO, obtener identificador (nunca 0)
  glBindBuffer(tipo_vbo, id_vbo); // activar el VBO usando su identificador
  // esta instrucción hace la transferencia de datos desde RAM hacia GPU
  glBufferData(tipo_vbo, tamanio_bytes, puntero_ram, GL_STATIC_DRAW);
  glBindBuffer(tipo_vbo, 0);
  return id_vbo;
}

void ObjMallaIndexada::draw_ModoDiferido()
{
  // (la primera vez, se deben crear los VBOs y guardar sus identificadores en el objeto)
  // completar (práctica 1)
  if (id_vbo_ver == 0)
    id_vbo_ver = CrearVBO(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data());

  if (id_vbo_tri == 0)
    id_vbo_tri = CrearVBO(GL_ELEMENT_ARRAY_BUFFER, triangulos.size() * 3 * sizeof(float), triangulos.data());

  // especificar localización y formato de la tabla de vértices, habilitar tabla

  glBindBuffer(GL_ARRAY_BUFFER, id_vbo_ver); // activar VBO de vértices
  glVertexPointer(3, GL_FLOAT, 0, 0);        // especifica formato y offset (=0)
  glBindBuffer(GL_ARRAY_BUFFER, 0);          // desactivar VBO de vértices.
  glEnableClientState(GL_VERTEX_ARRAY);      // habilitar tabla de vértices

  // visualizar triángulos con glDrawElements (puntero a tabla == 0)

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri); // activar VBO de triángulos
  glDrawElements(GL_TRIANGLES, 3 * triangulos.size(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // desactivar VBO de triángulos

  // desactivar uso de array de vértices
  glDisableClientState(GL_VERTEX_ARRAY);
}
// -----------------------------------------------------------------------------
// Función de visualización de la malla,
// puede llamar a  draw_ModoInmediato o bien a draw_ModoDiferido

void ObjMallaIndexada::draw(int modo_vis, bool GPU_mode)
{
  // completar .....(práctica 1)
  glColorPointer(3, GL_FLOAT, 0, colores.data());

  if (GPU_mode == 0)
    draw_ModoInmediato(modo_vis);
  else
    draw_ModoDiferido();
}

// -----------------------------------------------------------------------------
// Función de coloreo de la malla
void ObjMallaIndexada::colorear()
{
  for (int i = 0; i < vertices.size(); i++)
  {
    colores.push_back({0.9, 0.0, 0.2});
    colores.push_back({0.4, 0.0, 0.7});
    colores.push_back({0.3, 0.8, 0.1});
  }
}

//std::vector<Tupla3f> ObjMallaIndexada::getColores() { return colores; }

// -----------------------------------------------------------------------------
// Recalcula la tabla de normales de vértices (el contenido anterior se pierde)

void ObjMallaIndexada::calcular_normales()
{
  normales_vertices.clear();

  normales_vertices.resize(vertices.size(), {0.0, 0.0, 0.0});

  //Calculo de las normales de los triangulos
  Tupla3f vector_A, vector_B, vector_Normal;
  Tupla3f P0, P1, P2;

  for (int i = 0; i < triangulos.size(); ++i)
  {
    P0 = vertices[triangulos[i](0)];
    P1 = vertices[triangulos[i](1)];
    P2 = vertices[triangulos[i](2)];

    vector_A = P1 - P0;
    vector_B = P2 - P0;
    vector_Normal = vector_A.cross(vector_B);

    normales_vertices[triangulos[i](0)] = normales_vertices[triangulos[i](0)] + vector_Normal;
    normales_vertices[triangulos[i](1)] = normales_vertices[triangulos[i](1)] + vector_Normal;
    normales_vertices[triangulos[i](2)] = normales_vertices[triangulos[i](2)] + vector_Normal;
  }

  //Normalizamos la normales calculadas
  for (int i = 0; i < normales_vertices.size(); ++i)
    normales_vertices[i] = normales_vertices[i].normalized();
}

// *****************************************************************************
//
// Clase Cubo (práctica 1)
//
// *****************************************************************************

Cubo::Cubo()
{

  // inicializar la tabla de vértices
  vertices = {
      {-0.5, -0.5, -0.5}, // 0
      {-0.5, -0.5, +0.5}, // 1
      {-0.5, +0.5, -0.5}, // 2
      {-0.5, +0.5, +0.5}, // 3
      {+0.5, -0.5, -0.5}, // 4
      {+0.5, -0.5, +0.5}, // 5
      {+0.5, +0.5, -0.5}, // 6
      {+0.5, +0.5, +0.5}  // 7
  };
  // inicializar la tabla de caras o triángulos:
  // (es importante en cada cara ordenar los vértices en sentido contrario
  //  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

  triangulos = {
      {0, 2, 4}, {4, 2, 6}, {1, 5, 3}, {3, 5, 7}, {1, 3, 0}, {0, 3, 2}, {5, 4, 7}, {7, 4, 6}, {1, 0, 5}, {5, 0, 4}, {3, 7, 2}, {2, 7, 6}};

  material.activar();

  colorear();

  calcular_normales();
}

// *****************************************************************************
//
// Clase Tetraedro (práctica 1)
//
// *****************************************************************************

Tetraedro::Tetraedro()
{

  //Inicializar la tabla de vertices
  vertices = {
      {0, +1, 0},        // 0
      {+0.5, 0, 0},      // 1
      {0, 0, +0.5},      // 2
      {-0.25, 0, -0.25}, // 3
  };

  triangulos = {{0, 1, 3}, {1, 2, 3}, {2, 0, 3}, {0, 2, 1}};

  colorear();

  calcular_normales();

  material.activar();
}

// *****************************************************************************
//
// Clase Piramide (examen)
//
// *****************************************************************************

Piramide::Piramide()
{
  //Inicializar tabla de vertices
  vertices = {
      {+0.0, +0.0, +0.0},
      {-5.0, +0.0, +0.0},
      {-5.0, +0.0, -5.0},
      {+5.0, +0.0, -5.0},
      {+5.0, +0.0, +5.0},
      {+0.0, +0.0, +5.0},
      {+0.0, +5.0, +0.0},
      {+0.0, +0.0, -5.0},
      {+5.0, +0.0, +0.0},
  };

  //Inicializar tabla de triangulos
  triangulos = {
      {0, 6, 1}, {1, 6, 2}, {2, 6, 3}, {3, 6, 4}, {4, 6, 5}, {5, 6, 0}, {1, 2, 0}, {0, 2, 7}, {0, 7, 8}, {8, 7, 3}, {5, 0, 4}, {4, 0, 8}};

  //Coloreamos la figura
  colorear();

  calcular_normales();

  material.activar();
}
// *****************************************************************************
//
// Clase ObjPLY (práctica 2)
//
// *****************************************************************************

ObjPLY::ObjPLY(const std::string &nombre_archivo)
{
  // leer la lista de caras y vértices
  ply::read(nombre_archivo, vertices, triangulos);

  colorear();

  calcular_normales();

  material.activar();
}

// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************

inline Tupla3f rotar_vertice(Tupla3f vertice, int i, int N)
{
  const double Pi = 3.1415926535897;
  float x, y, z;
  Tupla3f vertice_rotado;
  float angulo = (2 * Pi * i / N);

  x = (cos(angulo) * vertice[0] + vertice[2] * sin(angulo));
  y = vertice[1];
  z = (-sin(angulo) * vertice[0] + cos(angulo) * vertice[2]);

  vertice_rotado = {x, y, z};

  return vertice_rotado;
}

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

ObjRevolucion::ObjRevolucion(const std::string &nombre_ply_perfil)
{
  // completar ......(práctica 2)
  std::vector<Tupla3f> perfil_original;

  ply::read_vertices(nombre_ply_perfil, perfil_original);

  this->crearMalla(perfil_original, 50, vertices, triangulos);

  colorear();

  calcular_normales();

  material.activar();
}

void ObjRevolucion::crearMalla(const std::vector<Tupla3f> perfil_original, const int num_instancias_perfil,
                               std::vector<Tupla3f> &vertices, std::vector<Tupla3i> &triangulos)
{
  int i, j, a, b, c, d;
  int num_vertices;
  int num_vertices_original = perfil_original.size();

  this->vertices.clear();
  this->triangulos.clear();

  //Crear tabla de vertices
  for (i = 0; i < num_instancias_perfil; i++)
  {
    for (j = 0; j < perfil_original.size(); j++)
    {
      Tupla3f v = rotar_vertice(perfil_original[j], i, num_instancias_perfil);
      this->vertices.push_back(v);
    }
  }

  num_vertices = vertices.size();

  //Crear table de triangulos
  for (i = 0; i < num_instancias_perfil; i++)
  {
    for (j = 0; j < perfil_original.size() - 1; j++)
    {
      a = perfil_original.size() * i + j;
      b = perfil_original.size() * ((i + 1) % num_instancias_perfil) + j;
      c = perfil_original.size() * i + (j + 1);
      d = perfil_original.size() * ((i + 1) % num_instancias_perfil) + (j + 1);

      this->triangulos.push_back({a, b, d});
      this->triangulos.push_back({a, d, c});
    }
  }

  //Creacion de la tapa inferior
  Tupla3f primer_ver = vertices[0];

  if (primer_ver[0] != 0)
  {
    vertices.push_back({0.0, primer_ver[1], 0.0});
    num_vertices++;

    for (i = 0; i + num_vertices_original < num_vertices; i += num_vertices_original)
      this->triangulos.push_back({i, num_vertices - 1, (i + num_vertices_original) % (num_vertices - 1)});
  }

  //Creacion de la tapa superior
  Tupla3f ult_ver = vertices[num_vertices - 2];

  if (ult_ver[0] != 0)
  {
    vertices.push_back({0.0, ult_ver[1], 0.0});
    num_vertices++;

    for (i = num_vertices_original - 1; i + num_vertices_original < num_vertices; i += num_vertices_original)
      this->triangulos.push_back({i, i + num_vertices_original, num_vertices - 1});

    //Ultimo triangulo (i = numn_instancias_perfil * num_vertices_original - 1)
    this->triangulos.push_back({i, num_vertices_original - 1, num_vertices - 1});
  }
}

// *****************************************************************************
//
// Clase Cono (práctica 2)
//
// *****************************************************************************
Cono::Cono(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

// *****************************************************************************
//
// Clase Cilindro (práctica 2)
//
// *****************************************************************************

Cilindro::Cilindro(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

// *****************************************************************************
//
// Clase Esfera (práctica 2)
//
// *****************************************************************************

Esfera::Esfera(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

void Material::activar()
{
  //Modificar reflectividad difusa, especular y ambiental
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambiental.data());
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_difuso.data());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_especular.data());

  //Modificar el exponente de brillo
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo * 128.0);
}

void ObjMallaIndexada::activar_Material()
{
  material.activar();
}