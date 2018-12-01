#include "aux.h" // includes de OpenGL/glut/glew, windows, y librería std de C++
#include "escena.h"
#include "malla.h" // objetos: Cubo y otros....

//**************************************************************************
// constructor de la escena (no puede usar ordenes de OpenGL)
//**************************************************************************

Escena::Escena()
{
    Front_plane = 0.1;
    Back_plane = 2000.0;
    Observer_distance = 2.0;
    Observer_angle_x = 0.0;
    Observer_angle_y = 0.0;

    ejes.changeAxisSize(5000);

    //Datos de la iluminacion
    float cdf_alfa[4] = {1.0, 1.0, 1.0, 1.0},
          caf_alfa[4] = {1.0, 1.0, 1.0, 1.0},
          cef_alfa[4] = {1.0, 1.0, 1.0, 1.0},
          pos_alfa[4] = {30.0, 30.0, 30.0, 0.0};

    //Magenta
    float cdf_beta[4] = {1.0, 0.0, 1.0, 1.0},
          caf_beta[4] = {0.0, 0.0, 0.0, 1.0},
          cef_beta[4] = {1.0, 0.0, 1.0, 1.0},
          pos_beta[4] = {30.0, 30.0, 30.0, 1.0};

    // crear los objetos de las prácticas: Mallas o Jerárquicos....
    cubo = new Cubo();
    tetraedro = new Tetraedro();
    PLY = new ObjPLY("plys/big_dodge.ply");
    Rev = new ObjRevolucion("plys/peon.ply");
    cilindro = new Cilindro("plys/cilindro.ply");
    esfera = new Esfera("plys/esfera.ply");
    cono = new Cono("plys/cono.ply");
    jerarquico = new ObjJerarquico();
    piramide = new Piramide();

    foco = new Luz(cdf_alfa, cef_alfa, caf_alfa, pos_alfa);
    foco->nueva_luz(cdf_beta, cef_beta, caf_beta, pos_beta);

    num_objetos = 9; // se usa al pulsar la tecla 'O' (rotar objeto actual)
}

//**************************************************************************
// inicialización de la escena (se ejecuta cuando ya se ha creado la ventana, por
// tanto sí puede ejecutar ordenes de OpenGL)
// Principalmemnte, inicializa OpenGL y la transf. de vista y proyección
//**************************************************************************

void Escena::inicializar(int UI_window_width, int UI_window_height)
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // se indica cual sera el color para limpiar la ventana	(r,v,a,al)

    glEnable(GL_DEPTH_TEST); // se habilita el z-bufer

    redimensionar(UI_window_width, UI_window_height);
}

// **************************************************************************
// Funcion que dibuja el objeto activo actual, usando su método 'draw'
// (llamada desde Escena::dibujar)
// ***************************************************************************

void Escena::dibujar_objeto_actual(bool shade_model)
{
    using namespace std;

    // (1) configurar OpenGL para el modo actual (puntos/lineas/sólido)
    //    llamar glPolygonMode, glColor... (y alguna cosas más), según dicho modo
    // .........completar (práctica 1)
    switch (mode)
    {
    case 0: //Muestra las lineas (modo lineas)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 1: //Muestra el relleno (modo relleno)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnable(GL_NORMALIZE);

        if(shade_model)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);

        foco->activar();
        break;

    case 2: //Muestra los puntos (modo puntos)
        foco->desactivar();
        glDisableClientState(GL_COLOR_ARRAY);
        glPointSize(5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;

    case 3: //Muestra triangulos de distinto color (modo ajedrez)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }

    // (2) dibujar el objeto actual usando método 'draw' del objeto asociado al
    // valor entero en 'objeto_actual'

    switch (objeto_actual)
    {
    case 0:
        if (cubo != nullptr)
        {
            cubo->draw(mode, cambia_modo);
            cubo->activar_Material();
        }
        break;
    case 1:
        if (tetraedro != nullptr)
        {
            tetraedro->draw(mode, cambia_modo);
            tetraedro->activar_Material();
        }
        break;
    case 2:
        if (PLY != nullptr)
        {
            PLY->draw(mode, cambia_modo);
            PLY->activar_Material();
        }
        break;
    case 3:
        if (Rev != nullptr)
        {
            Rev->draw(mode, cambia_modo);
            Rev->activar_Material();
        }
        break;
    case 4:
        if (cilindro != nullptr)
        {
            cilindro->draw(mode, cambia_modo);
            cilindro->activar_Material();
        }
        break;
    case 5:
        if (esfera != nullptr)
        {
            esfera->draw(mode, cambia_modo);
            esfera->activar_Material();
        }
        break;
    case 6:
        if (cono != nullptr)
        {
            cono->draw(mode, cambia_modo);
            cono->activar_Material();
        }
        break;
    case 7:
        if (jerarquico != nullptr)
        {
            jerarquico->draw(mode, cambia_modo);
        }
        break;
    case 8:
        if (piramide != nullptr)
        {
            piramide->draw(mode, cambia_modo);
            piramide->activar_Material();
        }
        break;
    default:
        cout << "draw_object: el número de objeto actual (" << objeto_actual << ") es incorrecto." << endl;
        break;
    }
}

// **************************************************************************
//
// función de dibujo de la escena: limpia ventana, fija cámara, dibuja ejes,
// y dibuja los objetos
//
// **************************************************************************

void Escena::dibujar()
{
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar la pantalla
    change_observer();
    ejes.draw();
    dibujar_objeto_actual(shade_model);
}

//**************************************************************************
//
// función que se invoca cuando se pulsa una tecla
// Devuelve true si se ha pulsado la tecla para terminar el programa (Q),
// devuelve false en otro caso.
//
//**************************************************************************

bool Escena::teclaPulsada(unsigned char tecla, int x, int y)
{
    using namespace std;
    cout << "Tecla pulsada: '" << tecla << "'" << endl;

    switch (toupper(tecla))
    {
    case 'Q':
        // salir
        return true;
        break;
    case 'O':
        // activar siguiente objeto
        objeto_actual = (objeto_actual + 1) % num_objetos;
        cout << "objeto actual == " << objeto_actual << endl;
        break;
    case 'M':
        // cambiar el modo de vista
        if (cambia_modo == false)
            mode = (mode + 1) % 4;
        else
            mode = (mode + 1) % 3;
        break;
    case 'V':
        //cambia el modo de vista
        if (cambia_modo == false)
        {
            cambia_modo = true;
            cout << "Cambio a modo diferido" << endl;
        }
        else
        {
            cambia_modo = false;
            cout << "Cambio a modo inmediato" << endl;
        }
        break;
    case 'P':
        // activar el parametro siguiente al actual
        if (jerarquico != nullptr && objeto_actual == 7)
            jerarquico->siguienteParametro();
        break;
    case 'A':
        // activar/desactivar las animaciones
        if (jerarquico != nullptr && objeto_actual == 7)
        {
            activarAnimaciones = !activarAnimaciones;
            this->conmutarAnimaciones();
        }
        break;
    case 'Z':
        // incrementa el valor del parametro actual del objeto jerarquico
        if (jerarquico != nullptr && objeto_actual == 7)
            jerarquico->incrementaParamAct();
        break;
    case '>':
        // incrementa el valor de incremento/decremento usado para las animaciones
        if (jerarquico != nullptr && objeto_actual == 7)
            jerarquico->acelerar();
        break;
    case '<':
        // decrementa el valor de incremento/decremento usado para las animaciones
        if (jerarquico != nullptr && objeto_actual == 7)
            jerarquico->decelerar();
        break;
    case 'I':
        luz = !luz;
        this->conmutarAnimaciones();
        break;
    case 'S':
        shade_model = !shade_model;
        break;
    }
    return false;
}
//**************************************************************************

void Escena::teclaEspecial(int Tecla1, int x, int y)
{
    switch (Tecla1)
    {
    case GLUT_KEY_LEFT:
        Observer_angle_y--;
        break;
    case GLUT_KEY_RIGHT:
        Observer_angle_y++;
        break;
    case GLUT_KEY_UP:
        Observer_angle_x--;
        break;
    case GLUT_KEY_DOWN:
        Observer_angle_x++;
        break;
    case GLUT_KEY_PAGE_UP:
        Observer_distance *= 1.2;
        break;
    case GLUT_KEY_PAGE_DOWN:
        Observer_distance /= 1.2;
        break;
    }

    //std::cout << Observer_distance << std::endl;
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//
// ratio_xy : relacción de aspecto del viewport ( == ancho(X) / alto(Y) )
//
//***************************************************************************

void Escena::change_projection(const float ratio_xy)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float wy = 0.84 * Front_plane;
    const float wx = ratio_xy * wy;
    glFrustum(-wx, +wx, -wy, +wy, Front_plane, Back_plane);
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar(int newWidth, int newHeight)
{
    Width = newWidth;
    Height = newHeight;
    change_projection(float(Width) / float(Height));
    glViewport(0, 0, Width, Height);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void Escena::change_observer()
{
    // posicion del observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -Observer_distance);
    glRotatef(Observer_angle_x, 1.0, 0.0, 0.0);
    glRotatef(Observer_angle_y, 0.0, 1.0, 0.0);
}

//***************************************************************************
// Funcion encargada de actualizar el estado de los parametros del
// objeto jerarquico
//***************************************************************************

void Escena::mgeDesocupado()
{
    if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
        foco->incrementa_angulo();

    if(objeto_actual == 7)
        jerarquico->actualizarEstado();
    glutPostRedisplay();
}

//***************************************************************************
// Funcion encargada de actualizar el estado de los parametros del
// objeto jerarquico
//***************************************************************************

void Escena::conmutarAnimaciones()
{
    if (activarAnimaciones)
    {
        jerarquico->inicioAnimaciones();
        glutIdleFunc(funcion_desocupado);
    }
    else if (luz)
    {
        glutIdleFunc(funcion_desocupado);
    }
    else
        glutIdleFunc(nullptr);
}

//***************************************************************************
// Constructor de un objeto Luz
//***************************************************************************
Luz::Luz(float cdf[], float cef[], float caf[], float posf[])
{
    Foco miFoco;

    for (int i = 0; i < 4; ++i)
    {
        miFoco.color_ambiental[i] = caf[i];
        miFoco.color_difuso[i] = cdf[i];
        miFoco.color_especular[i] = cef[i];
        miFoco.pos[i] = posf[i];
    }

    datos_luces.push_back(miFoco);
}

//***************************************************************************
// Funcion encargada de activar la iluminacion de la escena
//***************************************************************************

void Luz::activar() //Cambiar
{
    using namespace std;

    //Habilitamos iluminacion
    glEnable(GL_LIGHTING);

    for (int i = 0; i < datos_luces.size(); ++i)
    {
        //Habilitamos luz 0
        glEnable(luces[i]);

        //Configuracion del color de la fuente
        glLightfv(luces[i], GL_AMBIENT, datos_luces[i].color_ambiental);
        glLightfv(luces[i], GL_DIFFUSE, datos_luces[i].color_difuso);
        glLightfv(luces[i], GL_SPECULAR, datos_luces[i].color_especular);

        //Configuracion de la posicion de la fuente

        if (i == 1)
        {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
                glLoadIdentity();
                glMultMatrixd(translate);
                glRotatef(angulo_rotacion, 0.0, 1.0, 0.0);
                glLightfv(luces[i], GL_POSITION, datos_luces[i].pos);
                cout << "Angulo rotado: " << angulo_rotacion << endl;
            glPopMatrix();
        }
        else
            glLightfv(luces[i], GL_POSITION, datos_luces[i].pos);

    }
}

//***************************************************************************
// Funcion encargada de deactivar la iluminacion de la escena
//***************************************************************************

void Luz::desactivar()
{
    glDisable(GL_LIGHTING);
}

//***************************************************************************
// Funcion encargada de añadir una nueva luz
//***************************************************************************
void Luz::nueva_luz(float cdf[], float cef[], float caf[], float posf[])
{
    Foco nuevo;

    for (int i = 0; i < 4; ++i)
    {
        nuevo.color_ambiental[i] = caf[i];
        nuevo.color_difuso[i] = cdf[i];
        nuevo.color_especular[i] = cef[i];
        nuevo.pos[i] = posf[i];
    }

    datos_luces.push_back(nuevo);
}

//***************************************************************************
// Funcion encargada de incrementar el angulo de rotacion de la luz 
//***************************************************************************
void Luz::incrementa_angulo()
{
    angulo_rotacion += 0.5;
}