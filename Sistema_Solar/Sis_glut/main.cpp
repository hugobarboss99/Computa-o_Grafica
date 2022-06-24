#include<GL/glut.h>
#include <math.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/gl.h>
#define PI 3.1415926535897932384626433832795

static int raiomercurio = 200;
static int raiovenus = raiomercurio + 150;
static int raioterra = raiovenus + 150;
static int raiomarte = raioterra + 150;
static int raiojupiter = raiomarte + 200;
static int raiosaturno = raiojupiter + 200;
static int raiourano = raiosaturno + 150;
static int raionetuno = raiourano + 150;

static float mercurio = 360.0 / 87.97;
static float venus = 360.0 / 224.70;
static float terra = 360 / 365.26;
static float marte = 360.0 / 686.98;
static float jupiter = 360 / 4332.82;
static float saturno =  360 / 10755.7;
static float urano = 360 / 306871.5;
static float netuno = 360 / 601900.3;

static float angulomercurio = 0.0f;
static float angulovenus = 0.0f;
static float anguloterra = 0.0f;
static float angulomarte = 0.0f;
static float angulojupiter = 0.0f;
static float angulosaturno = 0.0f;
static float angulourano = 0.0f;
static float angulonetuno = 0.0f;

float eyeX = 0.0f;
float eyeY = 800.0f;
float eyeZ = 1700.0f;

GLfloat mat_ambiente[] = {0.7, 0.7, 0.7, 1.0};
GLfloat mat_specular[] = {0.0, 0.0, 0.0, 0.0};
GLfloat mat_brilho[] = {50.0};
GLfloat light_posicao[] = {1.0, 1.0, 1.0, 1.0};
GLfloat model_ambiente[] = {1.0, 1.0, 1.0, 1.0};

void materiais()
{
glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambiente);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_brilho);
glLightfv(GL_LIGHT0, GL_POSITION, light_posicao);
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambiente);
}

void mudacor(GLfloat r, GLfloat g, GLfloat b, GLfloat A)
{
    model_ambiente[0] = r;
    model_ambiente[1] = g;
    model_ambiente[2] = b;
    model_ambiente[3] = A;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambiente);
}

void mudatamanho(int w, int h)
{
   if(h == 0) h =1;
   float ratio = 1.0 * w / h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glViewport(0, 0, w, h);
   gluPerspective(45, ratio, 1, 5000);

   glMatrixMode(GL_MODELVIEW);
}

float angulo = 0.0f;
float j = 0.0f;;

void renderiza(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, eyeY, eyeZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    mudacor(1.0, 1.0, 1.0, 1.0);

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raiomercurio)
        glVertex3f(cos(i) * raiomercurio, 1.0, sin(i) * raiomercurio);
    glEnd();

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raiovenus)
        glVertex3f(cos(i) * raiovenus, 1.0, sin(i) * raiovenus);
    glEnd();

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raioterra)
        glVertex3f(cos(i) * raioterra, 1.0, sin(i) * raioterra);
    glEnd();

        glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raiomarte)
        glVertex3f(cos(i) * raiomarte, 1.0, sin(i) * raiomarte);
    glEnd();

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raiojupiter)
        glVertex3f(cos(i) * raiojupiter, 1.0, sin(i) * raiojupiter);
    glEnd();

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raiosaturno)
        glVertex3f(cos(i) * raiosaturno, 1.0, sin(i) * raiosaturno);
    glEnd();

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raiourano)
        glVertex3f(cos(i) * raiourano, 1.0, sin(i) * raiourano);
    glEnd();

    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i+= PI / raionetuno)
        glVertex3f(cos(i) * raionetuno, 1.0, sin(i) * raionetuno);
    glEnd();

    //Sol
    glPushMatrix();
    glRotatef(angulo, 0.0f, 1.0f, 0.0f);
    mudacor(1.0, 1.0, 0.0, 0.0);
    GLUquadric *sol;
    sol = gluNewQuadric();
    gluSphere(sol, 100, 100, 100);
    glPopMatrix();

    //Mercurio
    glPushMatrix();
    glRotatef(angulomercurio, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiomercurio, 0.0, 0.0);
    mudacor(66/255.0, 61/255.0, 57/255.0, 0.0);
    GLUquadric *Mercurio;
    Mercurio = gluNewQuadric();
    gluSphere(Mercurio, 10, 100, 100);
    glPopMatrix();

    //Venus
    glPushMatrix();
    glRotatef(angulovenus, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiovenus, 0.0, 0.0);
    mudacor(140.0/255.0, 70.0/255.0, 0.0, 0.0);
    GLUquadric *Venus;
    Venus = gluNewQuadric();
    gluSphere(Venus, 25.8, 100, 100);
    glPopMatrix();

    //Terra
    glPushMatrix();
    glRotatef(anguloterra, 0.0f, 1.0f, 0.0f);
    glTranslatef(raioterra, 0.0, 0.0);
    mudacor(0.0, 0.007, 0.686, 0.0);
    GLUquadric *Terra;
    Terra = gluNewQuadric();
    gluSphere(Terra, 20.6, 100, 100);


    //Lua
    glRotatef(5*anguloterra, 0.0f, 1.0f, 0.0f);
    glTranslatef(50, 0.0, 0.0);
    mudacor(0.7, 0.7, 0.7, 0.0);
    GLUquadric *Lua;
    Lua = gluNewQuadric();
    gluSphere(Lua, 7, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glRotatef(anguloterra, 0.0f, 1.0f, 0.0f);
    glTranslatef(raioterra, 0.0, 0.0);
    mudacor(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for(double i = 0; i < 2 * PI; i += PI / 1000) glVertex3f(cos(i) * 50, 0.0, sin(i) * 50);
    glEnd();
    glPopMatrix();

    //Marte
    glPushMatrix();
    glRotatef(angulomarte, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiomarte, 0.0, 0.0);
    mudacor(170/255.0, 60/255.0, 5/255.0, 0.0);
    GLUquadric *Marte;
    Marte = gluNewQuadric();
    gluSphere(Marte, 20.6, 100, 100);
    glPopMatrix();

    //Jupiter
    glPushMatrix();
    glRotatef(angulojupiter, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiojupiter, 0.0, 0.0);
    glRotatef(angulo+angulo*4, 0.0f, 1.0f, 0.0f); //Giro no proprio eixo
    mudacor(53/255.0, 107/255.0, 112/255.0, 0.0);
    GLUquadric *Jupiter;
    Jupiter = gluNewQuadric();
    gluSphere(Jupiter, 64.5, 100, 100);
    glPopMatrix();

    //Saturno
    glPushMatrix();
    glRotatef(angulosaturno, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiosaturno, 0.0, 0.0);
    glRotatef(angulo+angulo*4, 0.0f, 1.0f, 0.0f);
    mudacor(165/255.0, 138/255.0, 38/255.0, 0.0);
    GLUquadric *Saturno;
    Saturno = gluNewQuadric();
    gluSphere(Saturno, 54, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glRotatef(angulosaturno, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiosaturno, 0.0, 0.0);
    glRotatef(80.0f, 1.0f, 0.0f, 0.0f); //Rotaciona de forma circular
    mudacor(163/255.0, 149/255.0, 79/255.0, 0.0);
    glBegin(GL_POINTS);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*70, sin(i)*70, 0.0);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*75, sin(i)*75, 0.0);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*80, sin(i)*80, 0.0);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*85, sin(i)*85, 0.0);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*90, sin(i)*90, 0.0);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*95, sin(i)*95, 0.0);
    for(double i = 0; i < 2 *PI; i+=PI / 1000) glVertex3f(cos(i)*100, sin(i)*100, 0.0);
    glEnd();
    glPopMatrix();

    //Urano
    glPushMatrix();
    glRotatef(angulourano, 0.0f, 1.0f, 0.0f);
    glTranslatef(raiourano, 0.0, 0.0);
    mudacor(53/255.0, 198/255.0, 198/255.0, 0.0);
    GLUquadric *Urano;
    Urano = gluNewQuadric();
    gluSphere(Urano, 22.5, 100, 100);
    glPopMatrix();

    //Netuno
    glPushMatrix();
    glRotatef(angulonetuno, 0.0f, 1.0f, 0.0f);
    glTranslatef(raionetuno, 0.0, 0.0);
    mudacor(5/255.0, 5/255.0, 186/255.0, 0.0);
    GLUquadric *Netuno;
    Netuno = gluNewQuadric();
    gluSphere(Netuno, 24, 100, 100);
    glPopMatrix();

    angulo += 0.2f;
    angulomercurio += mercurio / 10;
    angulovenus += venus / 10;
    anguloterra += terra / 10;
    angulomarte += marte / 10;
    angulojupiter += jupiter / 2;
    angulosaturno += saturno / 2;;
    angulourano += urano * 10;
    angulonetuno += netuno * 10;

    glutSwapBuffers();
}

void mouse(int botao, int estado, int x, int y)
{
    switch(botao){
    case GLUT_LEFT_BUTTON:
        if(estado == GLUT_DOWN)
            glutIdleFunc(renderiza);
        break;
    case GLUT_RIGHT_BUTTON:
        if(estado == GLUT_DOWN)
            glutIdleFunc(NULL);
        break;
    }
}

void keyboard (unsigned char key, int xx, int yy){
    switch (key){
case 27:
    exit(0);
    break;

    }
}

void keyboardSpecial(int key, int xx, int yy){
    switch(key){
case GLUT_KEY_UP:
    eyeY -= 10.0f;
    eyeZ -= 10.0f;
    break;
    case GLUT_KEY_DOWN:
    eyeY += 10.0f;
    eyeZ += 10.0f;
    break;
    }
}

void inicia(void){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    materiais();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);
}

int main(int argc, char * argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 500);
    glutCreateWindow("Sistema Solar");
    inicia();

    glutDisplayFunc(renderiza);
    glutReshapeFunc(mudatamanho);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

    glutMainLoop();
    return 0;
}
