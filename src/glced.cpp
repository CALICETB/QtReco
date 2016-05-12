#include "glced.h"

#include <QDebug>
#include <QMouseEvent>

#include <math.h>
#include <iostream>

//Specific to glced (see CED server in ilcsoft)
#define AXES_COLOR 0.2,0.2,0.8
#define AXES_LINE_SIZE 1.
#define WORLD_SIZE 1000.

double bgcolor[4] = {1.,1.,1.,0.};

static unsigned char x_bm[]={
    0xc3,0x42,0x66,0x24,0x24,0x18,
    0x18,0x24,0x24,0x66,0x42,0xc3
};
static unsigned char y_bm[]={
    0xc0,0x40,0x60,0x20,0x30,0x10,
    0x18,0x2c,0x24,0x66,0x42,0xc3
};
static unsigned char z_bm[]={
    0xff,0x40,0x60,0x20,0x30,0x10,
    0x08,0x0c,0x04,0x06,0x02,0xff
};

static GLfloat axe[][3]={
    { 0., 0., 0., },
    { WORLD_SIZE/2, 0., 0. },
    { 0., WORLD_SIZE/2, 0. },
    { 0., 0., WORLD_SIZE/2 }
};

//-----------------------------------------------------------------------------------------------

GLCED::GLCED(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
{
    time_interval = 0;
    rtri = rquad = 0.0f;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(Update()));
    m_timer->start(time_interval);

    rotationX = 45.0;
    rotationY = 0.0;
    rotationZ = 45.0;
    scaling = 1.0;
}

//-----------------------------------------------------------------------------------------------

GLCED::~GLCED()
{
    m_timer->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void GLCED::initializeGL()
{
    //initialisation OGL
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

//-----------------------------------------------------------------------------------------------

void GLCED::mousePressEvent(QMouseEvent *event)
{
    //register position mouse
    lastPos = event->pos();
}

//-----------------------------------------------------------------------------------------------

void GLCED::mouseMoveEvent(QMouseEvent *event)
{
    //Movement according to mouse
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
    if (event->buttons() & Qt::LeftButton) {
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        updateGL();
    } else if (event->buttons() & Qt::RightButton) {
        rotationX += 180 * dy;
        rotationZ += 180 * dx;
        updateGL();
    }
    lastPos = event->pos();
}

//-----------------------------------------------------------------------------------------------

void GLCED::wheelEvent(QWheelEvent *event)
{
    //Zoom
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}

//-----------------------------------------------------------------------------------------------

void GLCED::paintGL()
{
    //Rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Draw_Axes();
    swapBuffers();
}

//-----------------------------------------------------------------------------------------------

void GLCED::axe_arrow()
{
    //create axis arrow
    GLfloat k = WORLD_SIZE/this->height();
    glutSolidCone(8.*k,30.*k,16,5);
}

//-----------------------------------------------------------------------------------------------

void GLCED::Draw_Axes()
{
    //Draw x,y,z axis
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(scaling, scaling, scaling);
    glRotatef(rotationX, 1.0,0.0,0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    glColor3f(AXES_COLOR);
    glLineWidth(AXES_LINE_SIZE);

    glBegin(GL_LINES);
    glVertex3fv(axe[0]);
    glVertex3fv(axe[1]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3fv(axe[0]);
    glVertex3fv(axe[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3fv(axe[0]);
    glVertex3fv(axe[3]);
    glEnd();

    glColor3f(0.5,0.5,0.8);
    glPushMatrix();
    glTranslatef(WORLD_SIZE/2.-WORLD_SIZE/100.,0.,0.);
    glRotatef(90.,0.0,1.0,0.0);
    axe_arrow();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.,WORLD_SIZE/2.-WORLD_SIZE/100.,0.);
    glRotatef(-90.,1.0,0.,0.);
    axe_arrow();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.,0.,WORLD_SIZE/2.-WORLD_SIZE/100.);
    axe_arrow();
    glPopMatrix();

    glGetDoublev(GL_COLOR_CLEAR_VALUE, bgcolor);
    glColor3f(0.0f,0.0f,0.0f);

    glRasterPos3f(WORLD_SIZE/2.+WORLD_SIZE/8,0.,0.);
    glBitmap(8,12,4,6,0,0,x_bm);
    glRasterPos3f(0.,WORLD_SIZE/2.+WORLD_SIZE/8,0.);
    glBitmap(8,12,4,6,0,0,y_bm);
    glRasterPos3f(0.,0.,WORLD_SIZE/2.+WORLD_SIZE/8);
    glBitmap(8,12,4,6,0,0,z_bm);
}

//-----------------------------------------------------------------------------------------------

void GLCED::resizeGL(int width, int height)
{
    //resizing of OGL
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WORLD_SIZE*width/height,WORLD_SIZE*width/height,-WORLD_SIZE,WORLD_SIZE, -20*WORLD_SIZE,20*WORLD_SIZE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//-----------------------------------------------------------------------------------------------

void GLCED::Update()
{
    //update function
    updateGL();
}
