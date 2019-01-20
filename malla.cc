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
  glEnableClientState(GL_VERTEX_ARRAY);

  if (glIsEnabled(GL_TEXTURE_2D) == GL_TRUE)
  {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, coord_textura.data());
    //glVertexPointer(3, GL_FLOAT, 0, verticesP.data());
    //glDrawElements(GL_TRIANGLES, triangulosP.size() * 3, GL_UNSIGNED_INT, triangulosP.data());
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT, triangulos.data());
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

  if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normales_vertices.data());
  }

  if (modo_vis == 3)
    this->draw_ModoAjedrez();
  else
    glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT, triangulos.data());

  if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
    glDisableClientState(GL_NORMAL_ARRAY);

  glDisableClientState(GL_VERTEX_ARRAY);
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
  if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normales_vertices.data());
  }

  if (id_vbo_ver == 0)
    id_vbo_ver = CrearVBO(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data());

  if (id_vbo_tri == 0)
    id_vbo_tri = CrearVBO(GL_ELEMENT_ARRAY_BUFFER, triangulos.size() * 3 * sizeof(float), triangulos.data());

  // especificar localización y formato de la tabla de vértices, habilitar tabla
  glBindBuffer(GL_ARRAY_BUFFER, id_vbo_ver); // activar VBO de vértices
  glVertexPointer(3, GL_FLOAT, 0, 0);        // especifica formato y offset (=0)
  glBindBuffer(GL_ARRAY_BUFFER, 0);          // desactivar VBO de vértices.
  glEnableClientState(GL_VERTEX_ARRAY);      // habilitar tabla de vértices

  // visualizar triángulos con glDrawElements (puntero a tabla == 0)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri); // activar VBO de triángulos
  glDrawElements(GL_TRIANGLES, 3 * triangulos.size(), GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // desactivar VBO de triángulos

  // desactivar uso de array de vértices
  glDisableClientState(GL_VERTEX_ARRAY);
}
// -----------------------------------------------------------------------------
// Función de visualización de la malla,
// puede llamar a  draw_ModoInmediato o bien a draw_ModoDiferido

void ObjMallaIndexada::draw(int modo_vis, bool GPU_mode)
{
  // completar .....(práctica 1)
  glColorPointer(3, GL_FLOAT, 0, colores.data());

  if (!GPU_mode)
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

void Material::activar()
{
  //Modificar reflectividad difusa, especular y ambiental
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ref_ambiental.data());
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ref_difusa.data());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ref_especular.data());

  //Modificar el exponente de brillo
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo * 128.0);
}

void ObjMallaIndexada::activar_Material()
{
  material.activar();
}

void ObjMallaIndexada::cargar_imagen(const char *FileName)
{
  CImg<unsigned char> foto;
  unsigned char *r, *g, *b;

  foto.load(FileName);

  for (int y = foto.height(); y > 0; y--)
    for (int x = foto.width(); x > 0; x--)
    {
      unsigned char *r = foto.data(x, y, 0, 0);
      unsigned char *g = foto.data(x, y, 0, 1);
      unsigned char *b = foto.data(x, y, 0, 2);
      pixels.push_back(*r);
      pixels.push_back(*g);
      pixels.push_back(*b);
    }

  this->ancho = foto.width();
  this->alto = foto.height();

  std::cout << "Imagen cargada. Ancho: " << foto.width() << " Alto: " << foto.height() << " Nº pixeles: " << pixels.size() / 3 << std::endl;
}

void ObjMallaIndexada::PreparaTextura()
{
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);

  // Generamos el id de la textura y la activamos
  glGenTextures(1, &textura_id);
  glBindTexture(GL_TEXTURE_2D, textura_id);

  // Establecer parametros de la textura
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Utilizar el color de la textura
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // Transfiere los datos a la GPU
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
}

void ObjMallaIndexada::calcular_coord_tex()
{
  float alfa, beta;
  float u, v;

  int tam = vertices.size();

  for (int i = 0; i < tam; i++)
  {
    alfa = atan2(vertices[i](2), vertices[i](0));
    beta = atan2(vertices[i](1), sqrt(pow(vertices[i](0), 2) + pow(vertices[i](2), 2)));

    u = 0.5 + (alfa / (2 * 3.141516));
    v = 0.5 + (beta / 3.141516);

    coord_textura.push_back({u, v});
  }
}

// *****************************************************************************
//
// Clase Cubo
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

  triangulos = {
      {0, 2, 4}, {4, 2, 6}, {1, 5, 3}, {3, 5, 7}, {1, 3, 0}, {0, 3, 2}, {5, 4, 7}, {7, 4, 6}, {1, 0, 5}, {5, 0, 4}, {3, 7, 2}, {2, 7, 6}};

  material.activar();

  colorear();

  calcular_normales();

  /*cargar_imagen("./cuadros/imagen2.jpg");

  calcular_coord_tex();

  PreparaTextura();*/
}

// *****************************************************************************
//
// Clase Tetraedro
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

  /*cargar_imagen("./cuadros/imagen2.jpg");

  calcular_coord_tex();

  PreparaTextura();*/
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
      {+0.0, 0.0, +0.0},  // 0
      {+1.0, -1.0, -1.0}, // 1
      {+1.0, -1.0, +1.0}, // 2
      {-1.0, -1.0, +1.0}, // 3
      {-1.0, -1.0, -1.0}, // 4

      {0.0, +1.0, 0.0},     // 5
      {+0.75, -0.25, -0.75}, // 6
      {+0.75, -0.25, +0.75}, // 7
      {-0.75, -0.25, +0.75}, // 8
      {-0.75, -0.25, -0.75},  // 9

      {0.0, +1.5, 0.0},    // 10
      {+0.50, 0.5, -0.50}, // 11
      {+0.50, 0.5, +0.50}, // 12
      {-0.50, 0.5, +0.50}, // 13
      {-0.50, 0.5, -0.50},  // 14

      {0.0, +2.0, 0.0},    // 15
      {+0.25, 1.25, -0.25}, // 16
      {+0.25, 1.25, +0.25}, // 17
      {-0.25, 1.25, +0.25}, // 18
      {-0.25, 1.25, -0.25}  // 19
  };

  triangulos = {
      {0, 3, 2},
      {0, 2, 1},
      {0, 1, 4},
      {0, 4, 3},
      {2, 3, 1},
      {1, 3, 4},

      {5, 8, 7},
      {5, 7, 6},
      {5, 6, 9},
      {5, 9, 8},
      {7, 8, 6},
      {6, 8, 9},

      {10, 13, 12},
      {10, 12, 11},
      {10, 11, 14},
      {10, 14, 13},
      {12, 13, 11},
      {11, 13, 14},

      {15, 18, 17},
      {15, 17, 16},
      {15, 16, 19},
      {15, 19, 18},
      {17, 18, 16},
      {16, 18, 19}
  };

  //Coloreamos la figura
  colorear();

  cargar_imagen("./cuadros/abeto.jpg");
  calcular_coord_tex();
  PreparaTextura();
};

// *****************************************************************************
//
// Clase Cuadro
//
// *****************************************************************************
Cuadro::Cuadro()
{
  vertices = {
      {+10.0, -5.0, 0.0},
      {-10.0, -5.0, 0.0},
      {+10.0, 5.0, 0.0},
      {-10.0, 5.0, 0.0},
  };

  triangulos = {
      {0, 2, 1},
      {1, 2, 3},
  };

  coord_textura = {
      {0.0, 0.0},
      {1.0, 0.0},
      {0.0, 1.0},
      {1.0, 1.0},
  };

  CuadroEsquina();
  calcular_normales();
  cargar_imagen("./cuadros/imagen2.jpg");
  PreparaTextura();
};

void Cuadro::CuadroEsquina()
{
  verticesP = {
      {7.0, 3.0, 0.0},  //0
      {7.0, 5.0, 0.0},  //1
      {10.0, 3.0, 0.0}, //2
      {10.0, 5.0, 0.0}  //3
  };

  triangulosP = {
      {0, 3, 1}, {0, 2, 3}};

  colorear();
  calcular_normales();
  material.activar();
}

// *****************************************************************************
//
// Clase ObjPLY
//
// *****************************************************************************

ObjPLY::ObjPLY(const std::string &nombre_archivo)
{
  // leer la lista de caras y vértices
  ply::read(nombre_archivo, vertices, triangulos);

  colorear();

  calcular_normales();

  material.activar();

  /*cargar_imagen("./cuadros/imagen2.jpg");

  calcular_coord_tex();

  PreparaTextura();*/
}

// *****************************************************************************
//
// Clase ObjRevolucion
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
  // completar .....
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
// Clase Cono
//
// *****************************************************************************
Cono::Cono(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

// *****************************************************************************
//
// Clase Cilindro
//
// *****************************************************************************

Cilindro::Cilindro(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

// *****************************************************************************
//
// Clase Esfera
//
// *****************************************************************************

Esfera::Esfera(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil)  {}

/////////////////////////////////////////////////////////////////////////////////////////////////
