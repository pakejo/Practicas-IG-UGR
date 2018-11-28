

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
    float cdf_alfa[4] = {1.0,1.0,1.0,1.0}, 
          caf_alfa[4] = {1.0,1.0,1.0,1.0},
          cef_alfa[4] = {1.0,1.0,1.0,1.0},
          pos_alfa[4] = {10.0,10.0,-10.0,0.0};

    float cdf_beta[4] = {1.0,0.0,1.0,1.0}, 
          caf_beta[4] = {1.0,1.0,1.0,1.0},
          cef_beta[4] = {1.0,0.0,1.0,1.0},
          pos_beta[4] = {10.0,10.0,-10.0,1.0};


    // crear los objetos de las prácticas: Mallas o Jerárquicos....
    cubo = new Cubo();
    tetraedro = new Tetraedro();
    PLY = new ObjPLY("plys/ant.ply");
    Rev = new ObjRevolucion("plys/peon.ply");
    cilindro = new Cilindro("plys/cilindro.ply");
    esfera = new Esfera("plys/esfera.ply");
    cono = new Cono("plys/cono.ply");
    jerarquico = new ObjJerarquico();
    piramide = new Piramide();

    alfa = new Luz(cdf_alfa, cef_alfa, caf_alfa, pos_alfa);
    beta = new Luz(cdf_beta, cef_beta, caf_beta, pos_beta);

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

void Escena::dibujar_objeto_actual()
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
        break;

    case 2: //Muestra los puntos (modo puntos)
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
            cubo->draw(mode, cambia_modo);
        break;
    case 1:
        if (tetraedro != nullptr)
            tetraedro->draw(mode, cambia_modo);
        break;
    case 2:
        if (PLY != nullptr)
            PLY->draw(mode, cambia_modo);
        break;
    case 3:
        if (Rev != nullptr)
            Rev->draw(mode, cambia_modo);
        break;
    case 4:
        if (cilindro != nullptr)
            cilindro->draw(mode, cambia_modo);
        break;
    case 5:
        if (esfera != nullptr)
            esfera->draw(mode, cambia_modo);
        break;
    case 6:
        if (cono != nullptr)
            cono->draw(mode, cambia_modo);
        break;
    case 7:
        if (jerarquico != nullptr)
            jerarquico->draw(mode, cambia_modo);
        break;
    case 8:
        if (piramide != nullptr)
            piramide->draw(mode, cambia_modo);
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
    dibujar_objeto_actual();
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
            this->conmutarAnimaciones();
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
    jerarquico->actualizarEstado();
    glutPostRedisplay();
}

//***************************************************************************
// Funcion encargada de actualizar el estado de los parametros del
// objeto jerarquico
//***************************************************************************

void Escena::conmutarAnimaciones()
{
    if (objeto_actual == 7)
    {
        activarAnimaciones = !activarAnimaciones;

        if (activarAnimaciones == true)
        {
            jerarquico->inicioAnimaciones();
            glutIdleFunc(funcion_desocupado);
        }
        else
            glutIdleFunc(nullptr);
    }
}


//***************************************************************************
// Constructor de un objeto Luz
//***************************************************************************
Luz::Luz(float cdf[], float cef[], float caf[], float posf[])
{
    for(int i = 0; i < 4; ++i)
    {
        color_ambiental[i] = caf[i];
        color_difuso[i] = cdf[i];
        color_especular[i] = cef[i];
        pos[i] = posf[i];
    }
}


//***************************************************************************
// Funcion encargada de activar la iluminacion de la escena
//***************************************************************************

void Luz::activar()
{
    //Habilitamos iluminacion
    glEnable(GL_LIGHTING);

    //Habilitamos luz 0
    glEnable(GL_LIGHT0);

    //Configuracion del color de la fuente
    glLightfv(GL_LIGHT0, GL_AMBIENT, color_ambiental);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color_difuso);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color_especular);

    //Configuracion de la posicion de la fuente
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

//***************************************************************************
// Funcion encargada de deactivar la iluminacion de la escena
//***************************************************************************

void Luz::desactivar()
{
    glDisable(GL_LIGHTING);
}