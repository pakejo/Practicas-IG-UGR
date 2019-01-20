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

    // Camara
    Camara camara1({0.0, 0.0, +7.0}, 0);
    Camara camara2({0.0, 0.0, -7.0}, 1);

    camaras.push_back(camara1);
    camaras.push_back(camara2);

    // Datos de la iluminacion
    float cdf_alfa[4] = {1.0, 1.0, 1.0, 1.0},
          caf_alfa[4] = {1.0, 1.0, 1.0, 1.0},
          cef_alfa[4] = {1.0, 1.0, 1.0, 1.0},
          pos_alfa[4] = {30.0, 10.0, 30.0, 0.0}; //Direccional

    //Magenta
    float cdf_beta[4] = {1.0, 0.0, 1.0, 1.0},
          caf_beta[4] = {0.0, 0.0, 0.0, 1.0},
          cef_beta[4] = {1.0, 0.0, 1.0, 1.0},
          pos_beta[4] = {30.0, 10.0, 30.0, 1.0}; //Posicional

    // Luces
    foco = new Luz(cdf_alfa, cef_alfa, caf_alfa, pos_alfa);
    foco->nueva_luz(cdf_beta, cef_beta, caf_beta, pos_beta);

    // Objetos
    cubo = new Cubo();
    tetraedro = new Tetraedro();
    PLY = new ObjPLY("plys/f16.ply");
    Rev = new ObjRevolucion("plys/peon.ply");
    esfera = new Esfera("plys/esfera.ply");
    cilindro = new Cilindro("plys/cilindro.ply");
    cono = new Cono("plys/cono.ply");
    jerarquico = new ObjJerarquico();
    cuadro = new Cuadro();
    piramide = new Piramide();

    num_objetos = 4; // se usa al pulsar la tecla 'O' (rotar objeto actual)

    for(int i = 0; i<num_objetos; i++)
        vector_cambio.push_back(0);
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

    switch (mode)
    {
    case 0: //Muestra las lineas (modo lineas)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 1: //Muestra el relleno (modo relleno)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnableClientState(GL_COLOR_ARRAY);

        if (encender_luz)
        {
            glEnable(GL_NORMALIZE);

            if (shade_model)
                glShadeModel(GL_SMOOTH);
            else
                glShadeModel(GL_FLAT);

            foco->activar();
        }
        else
            foco->desactivar();
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

    switch (objeto_actual)
    {
    case 0:
        if (cubo != nullptr)
        {
            cubo->draw(mode, cambia_modo);
            cubo->activar_Material();
            //cubo->PreparaTextura();
        }
        break;
    case 1:
        if (tetraedro != nullptr)
        {
            tetraedro->draw(mode, cambia_modo);
            tetraedro->activar_Material();
            //tetraedro->PreparaTextura();
        }
        break;
    case 2:
        if (PLY != nullptr)
        {
            PLY->draw(mode, cambia_modo);
            PLY->activar_Material();
            //PLY->PreparaTextura();
        }
        break;
    case 3:
        if (Rev != nullptr)
        {
            Rev->draw(mode, cambia_modo);
            Rev->activar_Material();
            //Rev->PreparaTextura();
        }
        break;
    case 4:
        if (cilindro != nullptr)
        {
            cilindro->draw(mode, cambia_modo);
            cilindro->activar_Material();
            //cilindro->PreparaTextura();
        }
        break;
    case 5:
        if (esfera != nullptr)
        {
            esfera->draw(mode, cambia_modo);
            esfera->activar_Material();
            //esfera->PreparaTextura();
        }
        break;
    case 6:
        if (cono != nullptr)
        {
            cono->draw(mode, cambia_modo);
            cono->activar_Material();
            //cono->PreparaTextura();
        }
        break;
    case 7:
        if (jerarquico != nullptr)
        {
            jerarquico->draw(mode, cambia_modo);
        }
        break;
    case 8:
        if (cuadro != nullptr)
        {
            cuadro->draw(mode, cambia_modo);
            cuadro->PreparaTextura();
        }
        break;
    case 9:
        if (piramide != nullptr)
        {
            piramide->draw(mode, cambia_modo);
            piramide->PreparaTextura();
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
    //dibujar_objeto_actual();
    dibujaSeleccion();
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
        mode = (mode + 1) % 4;
        break;
    case 'V':
        //Se cambia el modo de dibujo de inmediato a diferido
        if (!mode)
        {
            cout << "Se cambia el modo de dibujo a inmediato " << endl;
            mode = true;
            cubo->draw(mode, cambia_modo);
            tetraedro->draw(mode, cambia_modo);
            PLY->draw(mode, cambia_modo);
            cilindro->draw(mode, cambia_modo);
            esfera->draw(mode, cambia_modo);
            Rev->draw(mode, cambia_modo);
            jerarquico->draw(mode, cambia_modo);
            cuadro->draw(mode, cambia_modo);
            piramide->draw(mode, cambia_modo);
        }
        else
        {
            cout << "Se cambia el modo de dibujo a diferido" << endl;
            mode = false;
            cubo->draw(mode, cambia_modo);
            tetraedro->draw(mode, cambia_modo);
            PLY->draw(mode, cambia_modo);
            cilindro->draw(mode, cambia_modo);
            esfera->draw(mode, cambia_modo);
            Rev->draw(mode, cambia_modo);
            jerarquico->draw(mode, cambia_modo);
            cuadro->draw(mode, cambia_modo);
            piramide->draw(mode, cambia_modo);
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
        // Activar animacion de la luz
        luz = !luz;
        this->conmutarAnimaciones();
        break;
    case 'S':
        // Cambia el modo de iluminacion
        shade_model = !shade_model;
        break;
    case 'L':
        encender_luz = !encender_luz;
        break;
    }
    return false;
}
//**************************************************************************

void Escena::teclaEspecial(int Tecla1, int x, int y)
{
    switch (Tecla1)
    {
    case GLUT_KEY_F1:
        camaraActiva = 0;
        std::cout <<"Activada camara perspectiva" <<std::endl;
        this->change_projection(float(Width) / float(Height));
        break;
    case GLUT_KEY_F2:
        camaraActiva = 1;
        std::cout <<"Activada camara ortogonal" <<std::endl;
        this->change_projection(float(Width) / float(Height));
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
    camaras[camaraActiva].setProyection(ratio_xy);
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camaras[camaraActiva].setObserver();
}

//***************************************************************************
// Funcion encargada de actualizar el estado de los parametros del
// objeto jerarquico
//***************************************************************************

void Escena::mgeDesocupado()
{
    if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
        foco->incrementa_angulo();

    if (objeto_actual == 7)
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
// Funcion encargada de comprobar si se ha pulsado boton del raton
//***************************************************************************

void Escena::clickRaton(int boton, int estado, int x, int y)
{
    // Movemos la camara
    if (boton == GLUT_RIGHT_BUTTON)
    {
        if (estado == GLUT_DOWN)
        {
            seMueveCamara = true;
            xant = x;
            yant = y;
        }
        else
        {
            seMueveCamara = false;
            xant = 0;
            yant = 0;
        }
    }
    // Hacemos zoom
    else if ((boton == GLUT_LEFT_BUTTON) && (estado == GLUT_DOWN))
    {
        seMueveCamara = false;
        seHaceZoom = true;
        zoomAnt = y;
    }
    // Se selcciona un objeto 
    else if((boton == GLUT_LEFT_BUTTON) && (estado == GLUT_UP))
        this->pick_color(x,y);
}

void Escena::ratonMovido(int x, int y)
{
    float dx, dy;

    if (seMueveCamara)
    {
        dx = xant - x;
        dy = yant - y;

        camaras[camaraActiva].mover(dx, dy);
    }
    else if (seHaceZoom)
    {
        dy = zoomAnt - y;

        camaras[camaraActiva].zoom(dy);
    }

    glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////////
void Escena::dibujaSeleccionFalso()
{
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE);

    glPushMatrix();
        glPushMatrix();
        glTranslatef(0, 1.4, -1);
        glScalef(0.3,0.3,0.3);
        glColor3ub(75, 0, 130);
        PLY->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glPushMatrix();
        glTranslatef(-1, 0.8, 1);
        glScalef(0.1,0.1,0.1);
        glColor3ub(0, 255, 127);
        esfera->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glPushMatrix();
        glTranslatef(-1, 1, -1);
        glColor3ub(25, 25, 112);
        cubo->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();


    glPushMatrix();
        glPushMatrix();
        glTranslatef(1, 0.5, 1);
        glColor3ub(64, 224, 208);
        tetraedro->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();
}


void Escena::dibujaSeleccion()
{
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE);

    glPushMatrix();
        glPushMatrix();
        glTranslatef(0, 1.4, -1);
        glScalef(0.3,0.3,0.3);
        color_obj.push_back({75,0,130});
        if(vector_cambio[0] == 1)
            glColor3ub(255,255,0);
        else
            glColor3ub(75,0,130);
        PLY->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glPushMatrix();
        glTranslatef(-1, 0.8, 1);
        glScalef(0.1,0.1,0.1);
        color_obj.push_back({0,255,127});
        if(vector_cambio[1] == 1)
            glColor3ub(255,255,0);
        else
            glColor3ub(0,255,127);
        esfera->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glPushMatrix();
        glTranslatef(-1, 1, -1);
        color_obj.push_back({25,25,112});
        if(vector_cambio[2] == 1)
            glColor3ub(255,255,0);
        else
            glColor3ub(25,25,112);
        cubo->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();


    glPushMatrix();
        glPushMatrix();
        glTranslatef(1, 0.5, 1);
        color_obj.push_back({64, 224, 208});
        if(vector_cambio[3] == 1)
            glColor3ub(255,255,0);
        else
            glColor3ub(64, 224, 208);
        tetraedro->draw(mode, cambia_modo);
        glPopMatrix();
    glPopMatrix();

}

void Escena::pick_color(int x, int y)
{
    GLint viewport[4];
    unsigned char pixel[3];
    int indice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    dibujaSeleccionFalso();

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadBuffer(GL_BACK);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte *)&pixel[0]);

    printf("valor X: %d, valor Y: %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);

    for(int i=0; i<num_objetos; i++)
        if( (int)pixel[0] == color_obj[i](0) && (int)pixel[1] == color_obj[i](1) && (int)pixel[2] == color_obj[i](2))
            indice = i;
    
    if((indice >= 0) && (indice <=num_objetos))
    {
        if(vector_cambio[indice] == 1)
            vector_cambio[indice] = 0;
        else
            vector_cambio[indice] = 1;
    }

    this->dibujaSeleccion();
    glutPostRedisplay();
}