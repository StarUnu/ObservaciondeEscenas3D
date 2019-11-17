#include <GL/glut.h>
#include<iostream>
#include<vector>
#include<math.h>
using namespace std;

#define Frac_Circ 50 // Cien fracciones de circulo
#define PI 3.1415926535897932
GLfloat xRotated, yRotated, zRotated;
int w=900;
int h=700;
bool fullscreen = false;
bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

int np=0;
float px[10000];
float py[10000];
float pz[10000];
vector<vector<float>> ptoCirc;
GLdouble mModel[16];

using namespace std;

vector<vector<float> > datos ;

void setPixel (float xCoord, float yCoord)
{
  glBegin (GL_POINTS);
  
  vector<float> punto;
  punto.push_back(xCoord);
  punto.push_back(yCoord);
  datos.push_back(punto);
  glColor3f(0.5,0.5,0.5);
  cout<<"Punto a graficar"<<xCoord<<" "<<yCoord<<endl;
  glVertex3f(xCoord, yCoord,0.0);
  glEnd ( );
}

void circlePlotPoints (float x, float y, float tx, float ty)
{
  setPixel (x + tx, y + ty);
  setPixel (x - tx, y + ty);
  setPixel (x + tx, y - ty);
  setPixel (x - tx, y - ty);
  setPixel (x + ty, y + tx);
  setPixel (x - ty, y + tx);
  setPixel (x + ty, y - tx);
  setPixel (x - ty, y - tx);
  glutPostRedisplay();
}

void plot_circle(){
  int tamano=datos.size();
  for(int i=0;i<tamano;i++)
    {
      glBegin(GL_POINTS);
      //glColor3f(ro,g,b);
    glVertex2f (datos[i][0],datos[i][1]);
    glEnd ( );
    }
}
void circleMidpoint2(float x, float y,int r){
  cout<<"realiza el circulo deberas"<<endl;
  float d,tempx,tempy;
  tempx=0;
  tempy=r;
  d=5/4-r;
  int ro,g,b;
  ro=1;g=0;
  b=0;
  circlePlotPoints(x,y,tempx,tempy);
  while (tempy>tempx){
    if(d<0){
      d=d+2*tempx+3;
      tempx+=0.25;
    }
    else{
      d=d+2*(tempx-tempy)+5;
      tempx+=0.25;
      tempy-=0.25;
    }
    circlePlotPoints(x,y,tempx,tempy);
  }
  glEnd ( );
}

void init(void)
{
glClearColor(0, 0, 0, 0);
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);
glClearDepth(1.0f);
}

int fabs(int derivadas){
  if (derivadas<0){
    return derivadas*-1;  
  }
  return derivadas;
}


void displayTrace() {
  int i;
  //glColor3f(0.6,.3,0.2) ;
  glColor3f(1.0,1.0,0.0);
  
  cout<<"NP"<<np<<endl;
  glBegin(GL_QUAD_STRIP);
  glVertex3f(0,0,0);
  glVertex3f(0.03,0,0);
  glVertex3f(0,py[1],pz[1]);
  glVertex3f(0.03,py[1],pz[1]);
  glEnd();
  //cout<<"primeros datos"<<py[0]<<"otro"<<pz[0]<<endl;
  
  for (i = 1; i < np; i++) {
    cout<<"puntox"<<px[i-1]<<"punty"<<py[i-1]<<"puntoz"<<pz[i-1]<<endl;
    //glVertex3f(px[i],py[i],pz[i]);

    glBegin(GL_QUAD_STRIP);
    float fstpx=px[i-1];
    float fstpy=py[i-1];
    glVertex3f(fstpx,fstpy-0.03,pz[i]);
    glVertex3f(fstpx+0.03,fstpy-0.03,pz[i]);
    glVertex3f(fstpx,fstpy,pz[i]);
    glVertex3f(fstpx+0.03,fstpy,pz[i]);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(px[i], py[i]-0.03,pz[i]);
    glVertex3f(fstpx+0.03,py[i]-0.03,pz[i]);
    glVertex3f(px[i],fstpy,pz[i]);
    glVertex3f(fstpx+0.03,fstpy,pz[i]);
    glEnd();
  }
  
  //glEnd();

  

  glColor3f(0.3,.6,0.2) ;
  glBegin(GL_POINTS);
  for (i = 0; i < np; i++) {
    //cout<<"puntox"<<px[i]<<"punty"<<py[i]<<"puntoz"<<pz[i]<<endl;
    glVertex3f(px[i],py[i],pz[i]);
  }

  glEnd();
}

void plot_pixel(float x, float y, float z)
{
  glBegin(GL_POINTS);
  glColor3f(1,0,0);
  x=y=z=0;
  cout<<"x="<<x<<"y="<<y<<"z="<<z<<endl;
  glVertex3f(x,y,z);
  glEnd();
  glFlush();
}

void displey(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(0.0f, 0.0f, 3.0f,0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
  glRotatef(xrot, 1.0f, 0.0f, 0.0f);
  glRotatef(yrot, 0.0f, 1.0f, 0.0f);
 
  glMatrixMode(GL_MODELVIEW);
  // clear the drawing buffer.
  glClear(GL_COLOR_BUFFER_BIT);

  
  glPointSize(4); 
  glColor3f(0.5,0.5,0.5);


  float x,y,r;
  x=0.5;
  y=0;
  r=0.5;
  
  
  
   for (int i = 0; i < Frac_Circ + 1; i++) { // +1 para cerrar
    vector<float> pto;
    pto.push_back(x + r * cos(2.0 * PI * i / Frac_Circ));
    pto.push_back(y + r * sin(2.0 * PI * i / Frac_Circ));
    pto.push_back(0.0);
    ptoCirc.push_back(pto);
  //glVertex3f( x + r * cos(2.0 * PI * i / Frac_Circ) ,y + r * sin(2.0 * PI * i / Frac_Circ), 0.0);
  }
  //glTranslatef(ptoCirc[0][0], 0.0, ptoCirc[0][2]);
  glColor3f(1,0,0);
  glBegin(GL_LINES); //y
  glVertex3f(ptoCirc[0][0], ptoCirc[0][1], 0.0f);
  glVertex3f(ptoCirc[0][0], 0.3f, 0.0f);
  glEnd();
  glColor3f(0,0,1);
  glBegin(GL_LINES);//x 
  glVertex3f(ptoCirc[0][0], ptoCirc[0][1], 0.0f);
  glVertex3f(ptoCirc[0][0]+0.3f, 0.0f, 0.0f);
  glEnd();

  glColor3f(0,1,0);
  glBegin(GL_LINES); //z
  glVertex3f(ptoCirc[0][0], 0.0f, 0.0f);
  glVertex3f(ptoCirc[0][0], 0.0f, 0.3f);
  glEnd();
  cout<<"x="<<ptoCirc[0][0]<<"y="<<ptoCirc[0][1]<<"z="<<ptoCirc[0][2]<<endl;
  glColor3f(1,1,1);
  glBegin(GL_LINE_STRIP);

      float aumentador=0;
      for(int j=0;j <ptoCirc.size();j++ ){
        if(j==0 ){
          j+=1;
          cout<<"OTROx="<<ptoCirc[j][0]<<"y="<<ptoCirc[j][1]<<"z="<<ptoCirc[j][2]<<endl;
        }
        //glTranslatef(ptoCirc[j][0],ptoCirc[j][1],ptoCirc[j][2]+i);
        if(j<=40){
          
         glVertex3f(ptoCirc[j][0],ptoCirc[j][1],ptoCirc[j][2]);
        }
        else{
         glVertex3f(ptoCirc[j][0],ptoCirc[j][1],ptoCirc[j][2]+aumentador); 
         aumentador=aumentador+0.02; 
        }
        
      }
   
  /*
  //uno a dos 
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < 4; i++) { // +1 para cerrar
    float aumentador=0;
    for(int j=0;j <ptoCirc.size();j++ ){
      glVertex3f(ptoCirc[j][0],ptoCirc[j][1],ptoCirc[j][2]+i);
      //aumentador=aumentador+0.02; 
    }
    //aumentador+=i;
  }
  glEnd();*/
  

  //if(np==0)
  //  {circleMidpoint2(0.0,0.0,1);}

  /*
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f,0.0f);
   glVertex3f(0.2f,0.2f,0.2f);
  glEnd();*/
  /*
  glPushMatrix();
  glMultMatrixd(mModel);
  glPopMatrix();
  displayTrace();*/

  /*
   glColor3f(0,1,1);
   glPointSize(4); 
   linea(-0.5f, -0.5f,-0.5f,0.5f,0.5f,0.5f);
   //lineaDDA_dos(-0.5f, -0.5f,-0.5f,0.8f,0.8f,0.8f);
    glColor3f(0,1,1);*/

  /* //verificando
   glBegin(GL_LINES); 
   glVertex3f(-0.5f, -0.5f,-0.5f);
   glVertex3f(0.8f,0.8f,0.8f);
   glEnd();*/
  
  glPushMatrix();
  glMultMatrixd(mModel);
  glPointSize(4); 
  glBegin(GL_POINTS);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glEnd();
  glPopMatrix();
  displayTrace();
  //glFlush();
  glutSwapBuffers();
}


void addPointToTrace() {
  int i;
  GLdouble m[16];
  glGetDoublev (GL_MODELVIEW_MATRIX, m);
  // print the matrix
  printf ("\nMatrix:\n");
  for (i = 0; i < 4; i++) {
  printf ("Row %i: %f \t%f \t%f \t%f \n",
  i+1, m[i+0],m[i+4],m[i+8],m[i+12]);
  }
  // if is the first point
  if (np == 0) { // add the first point
  px [0] = 0;
  py [0] = 0;
  pz [0] = 0;
  np++;
  }
  px [np] = m[0] * px [0] + m[4] * py [0] + m[8] * pz [0] + m[12];
  py [np] = m[1] * px [0] + m[5] * py [0] + m[9] * pz [0] + m[13];
  pz [np] = m[2] * px [0] + m[6] * py [0] + m[10] * pz [0] + m[14];
  printf ("Point %i: %f \t%f \t%f \n",
  np, px[np],py[np],pz[np]);
  np++;
}

void keyboard(int key, int x, int y) {
  if (key==GLUT_KEY_UP){//flecha arriba del teclado,traslación arriba
      /*glPushMatrix();
      glBegin(GL_LINES);
      glColor3f(0,0,1);
      glVertex3f(100.0f,100.0f,0.0f);
      glVertex3f(500.0f,100.0f,0.0f);
      glEnd();*/
      
      /*
      glPointSize(4); 
      glColor3f(0.5,0.5,0.5);
      glBegin(GL_LINES);
      glVertex3f(0.0f, 0.0f,0.0f);
      glVertex3f(0.2f,0.2f,0.2f);
      glEnd();*/
      

      //glTranslatef(0.0, 0.1, 0.00);//lo traslada** de una posicion a otra posicion
      addPointToTrace();
      glPointSize(4); 
      glBegin(GL_POINTS);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glEnd();

      
      
      /*
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glGetDoublev(GL_MODELVIEW_MATRIX, mModel);*/
      /*
      for(int i=1;i<=16;i++){
          cout<<mModel[i-1]<<" ";
        if(i%4==0 and i!=0)
          cout<<endl;
      }*/
      
      //glPopMatrix();
      glFlush(); 
      //glPopMatrix();
  }
  if (key==GLUT_KEY_DOWN){
    glTranslatef(0.0, -0.1, 0.00);//lo traslada** de una posicion a otra posicion
    glPointSize(4); 
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
  }

  if (key==GLUT_KEY_LEFT){
    glTranslatef(-0.1, 0.0, 0.00);//lo traslada** de una posicion a otra posicion
    glPointSize(4); 
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
  }

  if (key==GLUT_KEY_RIGHT){
    glPointSize(4); 
    glBegin(GL_POINTS);
    glVertex3f(0.1f, 0.0f, 0.0f);
    glEnd();
    glTranslatef(0.1, 0.0, 0.00);//lo traslada** de una posicion a otra posicion
  }
  addPointToTrace();
  glFlush();
}

void resize(int d,int o)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glViewport(0, 0, w, h);
gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    mouseDown = true;
    xdiff = x - yrot;
    ydiff = -y + xrot;
  }
  else
    mouseDown = false;
}

void mouseMotion(int x, int y)
{
  if (mouseDown)
  {
    yrot = x - xdiff;
    xrot = y + ydiff;
    glutPostRedisplay();
  }

}


void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0
     
    gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}

int main(int argc, char** argv){

glutInit(&argc, argv);
//we initizlilze the glut. functions
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowPosition(100, 100);
glutInitWindowSize(w,h);
glutCreateWindow(argv[0]);
init();
glutDisplayFunc(displey);
glutMouseFunc(mouse);
glutMotionFunc(mouseMotion);
glutReshapeFunc(resize);
glutSpecialFunc(keyboard);
glutMainLoop();
return 0;
}