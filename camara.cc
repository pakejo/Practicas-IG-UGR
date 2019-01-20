#include "camara.h"

Camara::Camara(Tupla3f eye, int tipo)
{
    this->eye = eye;
    this->at = {0.0, 0.0, 0.0};
    this->up = {0.0, 1.0, 0.0};

    this->observer_angle_x = 0.0;
    this->observer_angle_y = 0.0;
    this->xant = 0;
    this->yant = 0;
    this->zoomAnt = 0;

    this->tipo = tipo;

    if (tipo == 0)
    {
        this->left = -0.084;
        this->right = 0.084;
        this->bottom = -0.084;
        this->top = 0.084;
        this->near = 0.1;
        this->far = 2000.0;

        Zoom = std::abs(eye(2));
    }
    else
    {
        this->left = -eye(2);
        this->right = +eye(2);
        this->bottom = -eye(2);
        this->top = +eye(2);
        this->near = -20.0;
        this->far = 20.0;

        Zoom = std::abs(eye(2));
    }
}

void Camara::setProyection(const float ratio_xy)
{
    if (tipo == 0)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();   
        gluPerspective(60.0, ratio_xy, near, far);
        glMatrixMode(GL_MODELVIEW);
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(left, right, bottom, top, near, far);
        glMatrixMode(GL_MODELVIEW);
    }

    this->ratio_xy = ratio_xy;
}

void Camara::setObserver()
{
    gluLookAt(eye(0), eye(1), eye(2), at(0), at(1), at(2), up(0), up(1), up(2));

    if(tipo == 0)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();   
        glFrustum(-Zoom/100, +Zoom/100, -Zoom/100, +Zoom/100, near, far);
        glMatrixMode(GL_MODELVIEW);
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-Zoom, +Zoom, -Zoom, +Zoom, near, far);
        glMatrixMode(GL_MODELVIEW);
    }
    
    this->rotarX(observer_angle_x);
    this->rotarY(observer_angle_y);
}

void Camara::rotarX(float angle)
{
    // Al mover el raton en el eje x
    // el objeto rota sobre el eje y
    glRotatef(angle, 0.0, 1.0, 0.0);
}

void Camara::rotarY(float angle)
{
    // Al mover el raton en el eje y
    // el objeto rota sobre el eje x
    glRotatef(angle, 1.0, 0.0, 0.0);
}

void Camara::zoom(float factor)
{
    float aumento = 0.1;

    /*std::cout <<"zoomAnt: " <<zoomAnt <<std::endl;
    std::cout <<"factor: " <<factor <<std::endl;*/

    if (tipo == 0) //Proyeccion perspectiva
    {
        if (zoomAnt > factor)
            Zoom -= aumento;
        else
            Zoom += aumento;
    }
    else //Proyeccion ortogonal
    {
        if (zoomAnt > factor)
            Zoom -= aumento;     
        else
            Zoom += aumento;
    }

    zoomAnt = factor;

    std::cout <<"Zomm: " <<Zoom <<std::endl;
}

void Camara::mover(float x, float y)
{
    // Proporcion eje y
    float dy = std::abs(yant - y);

    // Proporcion eje x
    float dx = std::abs(xant - x);

    /*Comprobamos la direccion a la que
    /se mueve el raton y actualizamos
    los valores*/
    if (xant > x)
        observer_angle_x += dx;
    else
        observer_angle_x -= dx;

    if (yant > y)
        observer_angle_y += dy;
    else
        observer_angle_y -= dy;

    /*Actualizamos los parametros*/
    xant = x;
    yant = y;
}