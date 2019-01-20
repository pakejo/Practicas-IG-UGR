#include "luz.h"

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
            glRotatef(angulo_rotacion, 1.0, 0.0, 0.0);
            glLightfv(luces[i], GL_POSITION, datos_luces[i].pos);
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