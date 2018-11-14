// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.cc
// -- implementación del objeto jerárquico de la práctica 3
//
// #############################################################################

#include "grafoparam.h"
#include "math.h"

// -----------------------------------------------------------------------------
// constructor: crea mallas indexadas en los nodos terminales del grafo

GrafoParam::GrafoParam()
{
      cilindro = new Cilindro("plys/cilindro.ply");
      cubo = new Cubo();
      panel = new Cubo();
      cubo2 = new Cubo();
      esfera = new Esfera("plys/esfera.ply");
      cono = new Cono("plys/cono.ply");
}
// -----------------------------------------------------------------------------
// actualizar valor efectivo de un parámetro (a partir de su valor no acotado)

void GrafoParam::actualizarValorEfe(const unsigned iparam, const float valor_na)
{
      assert(iparam < num_parametros);

      using namespace std;
      //cout << "GrafoParam::actualizarValorEfe( " << iparam << ", " << valor_na << " )" << endl ;

      switch (iparam)
      {
      case 0:
            // angulo en grados de rotacion 1 (Flexo)
            // crece linealmente a 150 grados por segundo, inicialmente es 20 grados
            ag_rotacion_1 = 150.0*valor_na;
            break;
      case 1:
            // ángulo en grados de rotacion 2 (bombilla)
            ag_rotacion_2 =7.0 + 7.0*sin( 0.5*(2.0*M_PI*valor_na) );
            break;      
      case 2:
            // angulo en grados de rotacion del planeta
            // crece linealmente a 150 grados por seguundo, inicialmente es 2 grados
            ag_rotacion_3 = 2.0 + 15.0*valor_na;
            break; 
      case 3:
            // altura 2: rotacion del objeto sobre la esfera
            altura_2 = fmod( ((20.0 + 150.0*valor_na) *0.3 ) , 360.0);
            break;
            
      }
}

// -----------------------------------------------------------------------------
// visualización del objeto Jerárquico con OpenGL,
// mediante llamadas a los métodos 'draw' de los sub-objetos

void GrafoParam::draw(const int p_modo_vis, const bool p_usar_diferido)
{
      // asegurarnos que está inicializado ok

      assert(cubo != nullptr);
      assert(cilindro != nullptr);
      assert(esfera != nullptr);
      assert(panel != nullptr);
      assert(cono != nullptr);

      // guardar parametros de visualización para esta llamada a 'draw'
      // (modo de visualización, modo de envío)
      usar_diferido = p_usar_diferido;
      modo_vis = p_modo_vis;

      // dibujar objetos
      glPushMatrix();
            //examen();
            dibujo_completo();
      glPopMatrix();
}
// -----------------------------------------------------------------------------

/*void GrafoParam::examen()
{
      glPushMatrix();
            glTranslatef(0.0, 1.0, 0.0);
            glScalef(20.0, 2.0, 10.0);
            base->draw(modo_vis, usar_diferido);
      glPopMatrix();

      glPushMatrix();
            glRotatef(ag_rotacion_1, 0.0, 1.0, 0.0);

            glPushMatrix();
                  glRotatef(30.0, 0.0, 0.0, 1.0);
                  glTranslatef(0.0, 11.0, 0.0);
                  glScalef(2.0, 20.0, 2.0);
                  cubo1->draw(modo_vis, usar_diferido);
            glPopMatrix();

            glPushMatrix();
                  glRotatef(45.0, 0.0, 0.0, -1.0);
                  glTranslatef(-20.0, 13.0, 0.0);
                  glScalef(2.0, 18.0, 2.0);
                  cubo2->draw(modo_vis, usar_diferido);
            glPopMatrix();

            glPushMatrix();
                  glRotatef(ag_rotacion_2, 0.0, 0.0, 1.0);
                  glScalef(0.6, 0.8, 0.6);
                  glTranslatef(ag_rotacion_2, 30.0, 0.0);
                  cono->draw(modo_vis, usar_diferido);
            glPopMatrix();

      glPopMatrix();
*/
void GrafoParam::dibujo_completo()
{
      glPushMatrix();
            //Planeta
            planeta();

            glPushMatrix();
                  glRotatef(altura_2, 1.0, 0.0, 1.0);       //Animacion (Rotacion pleanta)
                  //Base del objeto
                  base();
                  //Creacion del lateral izquierda
                  lateral_izda();
                  //Creacion del lateral derecho
                  lateral_dcha();
                  //Creacion de la parte central
                  centro();
            glPopMatrix();
      glPopMatrix();
}


void GrafoParam::centro()
{
      glPushMatrix();
            glScalef(7.0, 7.0, 7.0);
            glTranslatef( 0.0, 4.8, 0.0 );
            cubo->draw(modo_vis, usar_diferido );
      glPopMatrix();          
}

void GrafoParam::lateral_izda()
{
      glPushMatrix();    
            //Creacion del aspa izquierda
            panel_izda();
            // Union aspa al centro
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glTranslatef(33.5, 5.5, 0.0);
            glScalef(0.05, 0.25 ,0.05);
            cilindro->draw(modo_vis, usar_diferido );
      glPopMatrix();
}

void GrafoParam::panel_izda()
{
      glPushMatrix();
            glTranslatef(-17.0, 33.5, 0.0);
            glRotatef(ag_rotacion_2, 1.0, 0.0, 0.0);  //Animacion (Oscilacion)
            glScalef(20.0, 0.1, 3.0);
            panel->draw(modo_vis, usar_diferido);
      glPopMatrix();
}

void GrafoParam::lateral_dcha()     
{
      glPushMatrix();    
            //Creacion del aspa izquierda
            panel_dcha();
            // Union aspa al centro
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glTranslatef(33.5, -5.5, 0.0);
            glScalef(0.05, 0.25 ,0.05);
            cilindro->draw(modo_vis, usar_diferido );
      glPopMatrix();
}

void GrafoParam::panel_dcha()
{
      glPushMatrix();
            glTranslatef(17.0, 33.5, 0.0);
            glRotatef(ag_rotacion_2, 1.0, 0.0, 0.0);  //Animacion (Oscilacion)
            glScalef(20.0, 0.1, 3.0);
            panel->draw(modo_vis, usar_diferido);
      glPopMatrix();
}

void GrafoParam::base()
{
      glPushMatrix();
            glRotatef(ag_rotacion_1, 0.0, 1.0, 0.0);  //Animacion (Rotacion)

            //Parabolica
            glPushMatrix();
                  glTranslatef(2.0, 25.0, 0.0);
                  glRotatef(60.0, 0.0, 0.0, 1.0);
                  glScalef(0.3, 0.4, 0.3);
                  cono->draw(modo_vis, usar_diferido);
            glPopMatrix();

            //Esfera
            glPushMatrix();
                  glTranslatef(0.0, 26.5, 0.0);
                  glScalef(0.15, 0.15, 0.15);
                  esfera->draw(modo_vis, usar_diferido);
            glPopMatrix();

      glPopMatrix();

      //Eje 2
      glPushMatrix();
            glTranslatef(0.0, 28.0, 0.0);
            glScalef(0.1, 0.1, 0.1);
            cilindro->draw(modo_vis, usar_diferido);
      glPopMatrix();

      //Eje 1
      glPushMatrix();
            glTranslatef(0.0, 29.5, 0.0);
            glScalef(0.2, 0.1, 0.2);
            cilindro->draw(modo_vis, usar_diferido);
      glPopMatrix();
}

void GrafoParam::planeta()
{
      glPushMatrix();
            glRotatef(ag_rotacion_3, 0.0, 1.0, 0.0);  //Animacion (rotacion)
            glScalef(3.0, 3.0, 3.0);
            esfera->draw(modo_vis, usar_diferido);
      glPopMatrix();
}
