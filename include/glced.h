/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef GLCED_H
#define GLCED_H

#include <EventDisplayWindow.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <QGLWidget>
#include <QTimer>

/*
 * GLCED Class
 * Implementation of EventDisplay in interplay with Marlin. Based on CED Server in ilcsoft.
 * TO BE IMPLEMENTED!!!!
*/

class GLCED : public QGLWidget
{
    Q_OBJECT
public:
    //Constructor
    GLCED(QWidget *parent = 0);
    //Destructor
    ~GLCED();

public slots:
    //Update rendering
    void Update();

protected:
    //init OGL
    void initializeGL();
    //render OGL
    void paintGL();
    //resize OGL
    void resizeGL(int width, int height);
    //Mouse Handleling
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //Zoom Handleling
    void wheelEvent(QWheelEvent *event);

private:
    //X,Y,Z Axis rendering
    void Draw_Axes();
    void axe_arrow();

    QTimer *m_timer;
    GLfloat rtri, rquad;
    int time_interval;
    //X rotation angle X
    GLfloat rotationX;
    //X rotation angle Y
    GLfloat rotationY;
    //X rotation angle Z
    GLfloat rotationZ;
    //Scaling factor
    GLfloat scaling;
    //Last Mouse position
    QPoint lastPos;
};

#endif // GLCED_H
