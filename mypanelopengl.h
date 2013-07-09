#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H



#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <QKeyEvent>
#include <QGLWidget>
#include <QMatrix4x4>
#include <QHash>
#include <QList>

/*
#include <QtOpenGL>
#include <QGLWidget>*/


#include "line.h"
#include "spline.h"
#include "cylinder.h"
//#include "camera.h"


#define CS40_NUM_PROGS 2

class MyPanelOpenGL : public QGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    virtual ~MyPanelOpenGL();

private:
    void createShaders(int i, QString vertName, QString fragName);
   // void createShaders();
    void destroyShaders(int i);
    //void destroyShaders();
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void updatePolyMode(int val);
    void updateModel();
    void updateAngles(int idx, qreal amt);
    qreal wrap(qreal amt);

   /* QGLShader *vertexShader;
    QGLShader *fragmentShader;
    QGLShaderProgram *shaderProgram;*/

    QList<cs40::Drawable*> shapeList;
    QList<cs40::Drawable> axisList;
    QList<vec2> pointList;
    QList<QVector2D> clickList;
    cs40::Line* horizontalAxis;
    cs40::Line* verticalAxis;

    //don't need with camera ?
    QMatrix4x4 m_matrix;

    vec3 curr_color;
    vec2 point;
    vec2 clickPoint;
    int pos_move;
    int width; // width of window
    int height; //height of window

    Spline* our_spline;

    QList<QVector2D> pts;
    int iteration_num;
    int space_allocation;
    cs40::Cylinder* cyl;

    // NEW CAMERA STUFF
    QMatrix4x4 m_camera;
    QMatrix4x4 m_model;
    QMatrix4x4 m_projection;
     vec3 m_angles;

    /* Shaders and programs */
    QGLShader *m_vertexShaders[CS40_NUM_PROGS];
    QGLShader *m_fragmentShaders[CS40_NUM_PROGS];
    QGLShaderProgram *m_shaderPrograms[CS40_NUM_PROGS];
    int m_curr_prog;

    int m_polyMode;
    int m_shapeMode;

    GLuint m_textureID;
   GLuint m_normalMapID;


signals:
    
public slots:

    void modeAddPoint();
    void modeDraw();

    void curvePoints(vec4 C_x, vec4 C_y);
    void curvePointsLast(vec4 C_x, vec4 C_y, int interation_num);
    void spinIt();
    void reset();


};
#endif // MYPANELOPENGL_H
