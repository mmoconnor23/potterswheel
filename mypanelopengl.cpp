#include "mypanelopengl.h"
#include "triangle.h"
#include "line.h"
#include "rectangle.h"
#include "circle.h"
#include "geomfun.h"
#include "spline.h"
#include <cmath>
#include <iostream>
#include "common/matrixstack.h"

using namespace std;
using namespace cs40;

typedef QVector2D vec2;
typedef QVector3D vec3;

typedef enum CS40_APP_MODE{
  NONE=0,
  ADD_POINT=44,
  DRAW=111
} app_mode_t;

app_mode_t m_currentMode;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent){

    for(int i=0; i<CS40_NUM_PROGS; i++){
        m_shaderPrograms[i]=NULL;
        m_vertexShaders[i]=NULL;
        m_fragmentShaders[i]=NULL;
    }

    shapeList = QList<Drawable*>();
    m_matrix.setColumn(3,QVector4D(-1,1,0,1));
    m_currentMode = ADD_POINT;
    curr_color = vec3(1.0, 0, 0);
    clickList = QList<QVector2D>();
    pos_move = 0;
    clickPoint = vec2(0,0);
    our_spline = NULL;
    verticalAxis = NULL;
    horizontalAxis = NULL;
    cyl = NULL;

    m_polyMode = 2;
    m_curr_prog = 0;


}


ostream& operator<<(ostream& str, const QVector2D& vec){
    str << (float) vec.x() << ", " << (float) vec.y();
    return str;
}

MyPanelOpenGL::~MyPanelOpenGL(){
    m_shaderPrograms[m_curr_prog]->release();

    int i;
    for (i = 0; i < shapeList.length(); i++){
        delete shapeList.takeAt(i);
    }

    for(int i=0; i<CS40_NUM_PROGS; i++){
        destroyShaders(i);
    }

}

void MyPanelOpenGL::initializeGL()
{
    glewInit(); //manually do this now that we aren't using QtOpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    updatePolyMode(m_polyMode);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    createShaders(0, "vshader.glsl", "fshader.glsl");
    createShaders(1, "vshader2.glsl", "fshader2.glsl");

    cyl = NULL;

    m_textureID = bindTexture(QPixmap("data/xtra/claypottexture.jpg"), GL_TEXTURE_2D);
    m_normalMapID = bindTexture(QPixmap("data/beady.jpg"), GL_TEXTURE_2D);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMapID);

    m_shaderPrograms[m_curr_prog]->bind();

    m_projection.perspective(40,1,1,8);
    m_camera.lookAt(vec3(0,0,3),vec3(0,0,0),vec3(0,-1.,0.));
    updateModel();


}

void MyPanelOpenGL::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
    width = w;
    height = h;
    m_matrix.setColumn(0,QVector4D(2./w,0,0,0));
    m_matrix.setColumn(1,QVector4D(0,-2./h,0,0));
    updateModel();
    updateGL();

}

void MyPanelOpenGL::paintGL()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!m_shaderPrograms[m_curr_prog]){return;}
    m_shaderPrograms[m_curr_prog]->bind();

   if(m_curr_prog == 1){


       QMatrix4x4 mview = m_camera*m_model;
       m_shaderPrograms[m_curr_prog]->setUniformValue("projection",m_projection);
       m_shaderPrograms[m_curr_prog]->setUniformValue("camera",m_camera);
       m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_model);
       m_shaderPrograms[m_curr_prog]->setUniformValue("modelView",mview);
       m_shaderPrograms[m_curr_prog]->setUniformValue("normalMatrix",mview.normalMatrix());
       m_shaderPrograms[m_curr_prog]->setUniformValue("Tex0",0);
       m_shaderPrograms[m_curr_prog]->setUniformValue("Tex1",1);
       m_shaderPrograms[m_curr_prog]->setUniformValue("lightPos",vec4(1.5,0,2,1.)); //in world coordinates
       if (cyl != NULL) {
                 cyl->draw(m_shaderPrograms[m_curr_prog]);
             }

        glFlush();
   }

   if(m_curr_prog == 0){
      m_shaderPrograms[m_curr_prog]->setUniformValue("modelView", m_matrix);

      int i;
      for (i =0; i< shapeList.length(); i++) {
          if (shapeList[shapeList.length() -1 -i]->isVisible()) {
              shapeList[shapeList.length() -1 -i]->draw(m_shaderPrograms[m_curr_prog]);
          }
      }
      if (our_spline != NULL) {
           our_spline->draw(m_shaderPrograms[m_curr_prog], iteration_num, space_allocation);
      }



      glFlush();

   }



}

void MyPanelOpenGL::createShaders(int i, QString vertName, QString fragName){

    destroyShaders(i); //get rid of any old shaders
    m_vertexShaders[i] = new QGLShader(QGLShader::Vertex);
    if (!m_vertexShaders[i]->compileSourceFile(vertName)){
        qWarning() << m_vertexShaders[i]->log();
    }

    m_fragmentShaders[i] = new QGLShader(QGLShader::Fragment);
    if(!m_fragmentShaders[i]->compileSourceFile(fragName)){
        qWarning() << m_fragmentShaders[i]->log();
    }

    m_shaderPrograms[i] = new QGLShaderProgram();
    m_shaderPrograms[i]->addShader(m_vertexShaders[i]);
    m_shaderPrograms[i]->addShader(m_fragmentShaders[i]);

    if(!m_shaderPrograms[i]->link()){
        qWarning() << m_shaderPrograms[i]->log() << endl;
    }
}

void MyPanelOpenGL::destroyShaders(int i){
    delete m_vertexShaders[i]; m_vertexShaders[i]=NULL;
    delete m_fragmentShaders[i]; m_fragmentShaders[i]=NULL;

    if(m_shaderPrograms[i]){
        m_shaderPrograms[i]->release();
        delete m_shaderPrograms[i]; m_shaderPrograms[i]=NULL;
    }
}


void MyPanelOpenGL::updatePolyMode(int val){
    GLenum mode=GL_NONE;
    if(val==0){mode=GL_POINT;}
    else if(val==1){mode=GL_LINE;}
    else if(val==2){mode=GL_FILL;}

    if(mode != GL_NONE){
        glPolygonMode(GL_FRONT_AND_BACK, mode);
    }
}


void MyPanelOpenGL::mousePressEvent(QMouseEvent *event){
    vec2 click(event->posF().x(),event->posF().y());
    QVector4D worldPoint(click, 0, 1);
    vec2 point = worldPoint.toVector2D();
    clickList.append(point);



    switch (m_currentMode)
    {

        case ADD_POINT:
            if (clickList.length() == 1) {
               pointList.append(clickList[0]);

               cs40::Circle*  circ = new Circle(clickList[0], 5);
               shapeList.prepend(circ);
               circ->setColor(curr_color);
               updateGL();
               clickList.clear();
            }
            break;

        default:
             cout << "default" << endl;

    }
}
/*
void MyPanelOpenGL::mouseMoveEvent(QMouseEvent *event){

    if(m_currentMode == MOVING && !(pos_move == -1)){

        vec2 move = vec2(event->pos().x(),event->pos().y());

        if(!(move.x() < 0) && !(move.x() > width) && !(move.y() > height)
                && !(move.y() < 0)){
          shapeList[pos_move]->move(move.x()-clickPoint.x(),move.y()-clickPoint.y());
          clickPoint = move;
          updateGL();
        }
    }


}
*/
void MyPanelOpenGL::keyPressEvent(QKeyEvent *event)
{
    qreal step=1;
    /*Enable strong Focus on GL Widget to process key events*/
    switch(event->key()){
    case Qt::Key_X:
        if (event->text()=="x"){updateAngles(0,step);}
        else{updateAngles(0,-step);}
        break;
    case Qt::Key_Y:
        if (event->text()=="y"){ updateAngles(1,step);}
        else{ updateAngles(1,-step);}
        break;
    case Qt::Key_Z:
        if (event->text()=="z"){updateAngles(2,step);}
        else{updateAngles(2,-step);}
        break;
    case Qt::Key_C:
        if(glIsEnabled(GL_CULL_FACE)){glDisable(GL_CULL_FACE);}
        else{glEnable(GL_CULL_FACE);}
        break;
    case Qt::Key_P:
        m_polyMode = (m_polyMode+1)%3;
        updatePolyMode(m_polyMode);
        break;
    case Qt::Key_S:
        m_shapeMode = (m_shapeMode+1)%6;
        break;
    case Qt::Key_V:
        m_curr_prog = (m_curr_prog+1)%CS40_NUM_PROGS;
        break;
    case Qt::Key_I:
        m_camera.translate(0,0,.25);
        break;
    case Qt::Key_O:
        m_camera.translate(0,0,-.25);
        break;
    case Qt::Key_Up:
        m_camera.translate(0,-.25,0);
        break;
    case Qt::Key_Down:
        m_camera.translate(0,.25,0);
        break;
    case Qt::Key_Left:
        m_camera.translate(.25,0,0);
        break;
    case Qt::Key_Right:
        m_camera.translate(-.25,0,0);
        break;
    default:
        QWidget::keyPressEvent(event); /* pass to base class */
    }
    updateGL();
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt){
    if(idx == 0){
        m_angles.setX(wrap(m_angles.x()+amt));
    }else if(idx == 1){
        m_angles.setY(wrap(m_angles.y()+amt));
    }else if(idx == 2){
        m_angles.setZ(wrap(m_angles.z()+amt));
    }
    updateModel();
}


qreal MyPanelOpenGL::wrap(qreal amt){
    if (amt > 360.){ return amt - 360.; }
    else if (amt < 0.){ return amt + 360.; }
    return amt;
}

void MyPanelOpenGL::updateModel(){
    m_model.setToIdentity();

    m_model.scale(1./width);
    m_model.rotate(m_angles.z(), vec3(0,0,1.));
    m_model.rotate(m_angles.y(), vec3(0,1,0.));
    m_model.rotate(m_angles.x(), vec3(1,0,0.));
}

void MyPanelOpenGL::modeAddPoint(){
    m_currentMode=ADD_POINT;
    clickList.clear();
}

void MyPanelOpenGL::modeDraw(){

    vec4 x_coords, y_coords, c_x, c_y;

    space_allocation = 0;

    QMatrix4x4 M = QMatrix4x4(   1,     0,      0,      0,
                              -5.5,     9,   -4.5,      1,
                                 9, -22.5,     18,   -4.5,
                              -4.5,  13.5,  -13.5,    4.5);


    x_coords = vec4(pointList[0].x(), pointList[1].x(),
                    pointList[2].x(), pointList[3].x());
    y_coords = vec4(pointList[0].y(), pointList[1].y(),
                    pointList[2].y(), pointList[3].y());

    c_x = M*x_coords;
    c_y = M*y_coords;

    curvePoints(c_x, c_y);

    iteration_num = 1;
    for (int i = 1; i < pointList.length()-3; i++) {
        x_coords = vec4(pointList[i].x(), pointList[i+1].x(),
                        pointList[i+2].x(), pointList[i+3].x());
        y_coords = vec4(pointList[i].y(), pointList[i+1].y(),
                        pointList[i+2].y(), pointList[i+3].y());

        c_x = M*x_coords;
        c_y = M*y_coords;

        space_allocation = 1;
        curvePointsLast(c_x, c_y, iteration_num);
        iteration_num++;

    }

    int length = pts.length();
    our_spline = new Spline(pts, iteration_num, length);

    updateGL();

}

void MyPanelOpenGL::curvePoints(vec4 C_x, vec4 C_y){

    double u;
    vec2 pt, c0, c1, c2, c3;
    int k = 60;
    c0 = vec2(C_x.x(), C_y.x());
    c1 = vec2(C_x.y(), C_y.y());
    c2 = vec2(C_x.z(), C_y.z());
    c3 = vec2(C_x.w(), C_y.w());

    for (int i = 0; i<=k;i++){
        u = ((double)i)/k;
        pt = c0 + c1*u + c2*u*u + c3*u*u*u;
        pts.insert(i, pt);

    }

    return;
}

void MyPanelOpenGL::curvePointsLast(vec4 C_x, vec4 C_y, int iteration_num){

    double u;
    vec2 pt, c0, c1, c2, c3;
    int k = 60;
    c0 = vec2(C_x.x(), C_y.x());
    c1 = vec2(C_x.y(), C_y.y());
    c2 = vec2(C_x.z(), C_y.z());
    c3 = vec2(C_x.w(), C_y.w());

    for (int i = 40; i<=k;i++){
        u = ((double)i)/k;
        pt = c0 + c1*u + c2*u*u + c3*u*u*u;
        pts.insert((k+1)+(20*(iteration_num-1))+(i-40), pt);

    }

    return;
}

void MyPanelOpenGL::spinIt(){

   cyl = new Cylinder(50,pts.length() , pts);
   m_curr_prog = 1;
   updateGL();

}

void MyPanelOpenGL::reset(){
pointList.clear();
clickList.clear();
our_spline = NULL;
cyl = NULL;

int i;
for (i = 0; i < shapeList.length(); i++){
// destructs object instances in shapeList
delete shapeList.takeAt(i);
}
shapeList.clear();

m_curr_prog = 0;

updateGL();
}
