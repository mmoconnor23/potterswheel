#include "line.h"
#include <iostream>
#include "geomfun.h"

using namespace cs40;

Line::Line(const vec2& p1, const vec2& p2){
    pts[0] = p1;
    pts[1] = p2;

    /* TODO: verify that points are in CCW order */

    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,2*sizeof(vec2));

   visible = true;
   m_visible=true;

}

Line::Line(const Line *other){
  /* implement copy constructor */
    pts[0] = other->pts[0];
    pts[1] = other->pts[1];

    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,2*sizeof(vec2));

    m_visible = true;
}

void Line::move(float dx, float dy){
  /* implement this.
   * update pts
   * write updates to old QGLBuffer
   */
    int i;
    for (i = 0; i < 2; i++){
        pts[i].setX(pts[i].x()+dx);
        pts[i].setY(pts[i].y()+dy);
    }

    m_vbo->bind();

    m_vbo->write(0, pts, 2*sizeof(vec2));
}

void Line::draw(QGLShaderProgram* prog){
    /* bind active vbo
     * set color uniform value in fragment shader
     * enable attribute array for vPosition input in vertex shader
     * describe layout of VBO for attribute array
     * glDrawArrays
     * release vbo
     */


   m_vbo->bind();
   prog->setUniformValue("color", m_color);
   prog->enableAttributeArray("vPosition");
   prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);
   glDrawArrays(GL_LINE_STRIP, 0, 2);
   m_vbo->release();





}

bool Line::contains(const vec2 &pt){


    float dist = distToSegment(pt,pts[0],pts[1]);
 vec2 v = pts[1] - pts[0] ;
vec2 w = pt - pts[0];


float scalar = QVector2D::dotProduct(v,w)/(v.lengthSquared());

    if(dist< 10 ){


        if( scalar < 1 && scalar > 0 ){
        return true;
        }
    }

    return false;

}


bool Line::isVisible(){

   return visible;

}




