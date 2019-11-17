#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "camera.h"
#include "camera.cpp"

#include "vector_tools.h"
#include "vector_tools.cpp"
#include <stdio.h>

static camera *LOCAL_MyCamera;
static int old_x, old_y;

using namespace std;
float pi2 = 6.28318530718;

void esferaBasica(float radio, int nlatitud, int nlongitud);
void Zoom(int x, int y){
  float zoom;
  zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
  old_y = y;
  switch(LOCAL_MyCamera->camMovimiento){
    case CAM_EXAMINAR:
      if (LOCAL_MyCamera->camAperture + zoom > (5 * DEGREE_TO_RAD) &&
      LOCAL_MyCamera->camAperture + zoom < 175 * DEGREE_TO_RAD)
      LOCAL_MyCamera->camAperture=LOCAL_MyCamera->camAperture + zoom;
      break;
  }
  glutPostRedisplay();
}


void Desplazar(int x, int y){
  float rotacion_x, avance_y;
  avance_y = (float)(y - old_y) / 10;
  rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD / 5;
  YawCamera( LOCAL_MyCamera, rotacion_x );
  //PitchCamera(LOCAL_MyCamera, 45 );
  AvanceFreeCamera( LOCAL_MyCamera, avance_y);
  old_y = y;
  old_x = x;
  glutPostRedisplay();
}


void Examinar(int x, int y){
  float rot_x, rot_y;
  rot_y = (float)(old_y - y);
  rot_x = (float)(x - old_x);
  Rotar_Latitud( LOCAL_MyCamera, rot_y * DEGREE_TO_RAD );
  Rotar_Longitud( LOCAL_MyCamera, rot_x * DEGREE_TO_RAD );
  old_y = y;
  old_x = x;
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
  old_x = x;
  old_y = y;
  if(button==GLUT_LEFT_BUTTON and state==GLUT_DOWN){
    cout<<"Zoom y Efecto de Yaw "<<endl;
    glutMotionFunc(Desplazar);
  }
  glutPostRedisplay();
}

void MouseMotion(int x, int y){
  old_y = y;
  old_x = x;
}

void keyboard ( int key, int x, int y ){
  cout<<"Imprimiendo"<<key<<endl;
  if(key==GLUT_KEY_F1){
      cout<<"Camara parada"<<endl;
      glutPassiveMotionFunc(MouseMotion);
      LOCAL_MyCamera->camMovimiento = CAM_STOP;
  }
  if(key==GLUT_KEY_F2){
      cout<<"Mover objeto en proyeccion"<<endl;
      glutPassiveMotionFunc(Examinar);
      LOCAL_MyCamera->camMovimiento = CAM_EXAMINAR;
  }
  if(key==GLUT_KEY_F3){
      cout<<"Mover camara con mouse"<<endl;
      glutPassiveMotionFunc(MouseMotion);
      LOCAL_MyCamera->camMovimiento = CAM_PASEAR;
      LOCAL_MyCamera->camAtY = 0;
      LOCAL_MyCamera->camViewY = 0;
      SetDependentParametersCamera( LOCAL_MyCamera );
  }
	if(key==GLUT_KEY_F4){
		if(LOCAL_MyCamera->camProjection == CAM_CONIC){
		LOCAL_MyCamera->x1=-5;//
		LOCAL_MyCamera->x2=7;////aumenta enflaquese
		LOCAL_MyCamera->y1=-5;//
		LOCAL_MyCamera->y2=6;//
		LOCAL_MyCamera->z1=-20;
		LOCAL_MyCamera->z2=500;
		LOCAL_MyCamera->camProjection = CAM_PARALLEL;
		cout<<"Modo de Proyección Ortogonal"<<endl;
		}else{
			cout<<"Modo de Proyección Perspectiva"<<endl;	
			LOCAL_MyCamera->camProjection = CAM_CONIC;
		}
  }
	if(key==GLUT_KEY_F7){ //Reset Camera
    cout<<"Reinicio"<<endl;
      LOCAL_MyCamera->camAtX =0;
      LOCAL_MyCamera->camAtY =0;
      LOCAL_MyCamera->camAtZ =0;
      LOCAL_MyCamera->camViewX = 0;
      LOCAL_MyCamera->camViewY = 1;
      LOCAL_MyCamera->camViewZ = -3;
      SetDependentParametersCamera( LOCAL_MyCamera );
  }
  if (key==GLUT_KEY_UP){//flecha arriba del teclado,traslación arriba
      cout<<"Movimiento Arriba"<<endl;
      PanCamera(LOCAL_MyCamera,0,-1);
  }
  if (key==GLUT_KEY_DOWN){
      cout<<"Movimiento Abajo"<<endl;
      PanCamera(LOCAL_MyCamera,0,1);
  }

  if (key==GLUT_KEY_LEFT){
      cout<<"Movimiento Izquierda"<<endl;
      PanCamera(LOCAL_MyCamera,1,0);
  }

  if (key==GLUT_KEY_RIGHT){
      cout<<"Movimiento Derecha"<<endl;
      PanCamera(LOCAL_MyCamera,-1,0);
  }

  glutPostRedisplay();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  SetGLAspectRatioCamera( LOCAL_MyCamera );
}

void esferaBasica(float radio, int nlatitud, int nlongitud)
{
     const float pi = 3.14159265358979323846;
     float inct, incf;
     int i, j;
     float vertice[3];
     inct = 2 * pi / nlatitud;
     incf = pi / nlatitud;
     for( i = 0; i < nlatitud; i++ )
     {
          glBegin( GL_LINE_STRIP );
          vertice[0] = vertice[1] = 0;
          vertice[2] =- radio;
          glVertex3fv( vertice );
          for( j = 1; j < nlongitud-1; j++ )
          {
               vertice[0] = radio * cos(i*inct) * cos(j*incf-0.5*pi);
               vertice[1] = radio * sin(i*inct) * cos(j*incf-0.5*pi);
               vertice[2] = radio * sin(j*incf-0.5*pi);
               glVertex3fv( vertice );
               vertice[0] = radio*cos((i+1)*inct)*cos(j*incf-0.5*pi);
               vertice[1] = radio*sin((i+1)*inct)*cos(j*incf-0.5*pi);
               glVertex3fv(vertice);
          }
          vertice[0] = vertice[1] = 0;
          vertice[2] = radio;
          glVertex3fv( vertice );
     }
     glEnd();
}

void displey(void)
{
    SetGLCamera( LOCAL_MyCamera );

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.7,0.8,0.2);
    glLoadIdentity();
    esferaBasica(2,10,10);
    glFlush();

}

int main(int argc, char** argv)
{
    LOCAL_MyCamera = CreatePositionCamera(5.0f, 0.0f, -10.0f);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Perspectivas Camara");
    glClearColor(0, 0, 0, 0);
    glutDisplayFunc(displey);
    glutMouseFunc(mouse);
    glutSpecialFunc(keyboard);
    glutPassiveMotionFunc(MouseMotion);
    
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

