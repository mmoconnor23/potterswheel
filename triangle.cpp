#include "triangle.h"
#include <iostream>
#include "geomfun.h"

using namespace cs40;


Triangle::Triangle(const vec2& p1, const vec2& p2, const vec2& p3){


    if (leftOf(p1,p2,p3)){
        pts[0] = p1;
        pts[1] = p2;
        pts[2] = p3;

    }

    else if(leftOf(p1,p3,p2)){
        pts[0] = p1;
        pts[1] = p3;
        pts[2] = p2;

    }

    // designed to catch weird occurrences
    else { std::cout << "wtf" << endl ;}




    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,3*sizeof(vec2));

    m_visible = true;
}

Triangle::Triangle(const Triangle *other){
  /* implement copy constructor */
    pts[0] = other->pts[0];
    pts[1] = other->pts[1];
    pts[2] = other->pts[2];

    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,3*sizeof(vec2));

    m_visible = true;
}

void Triangle::move(float dx, float dy){
  /* implement this.
   * update pts
   * write updates to old QGLBuffer
   */
    int i;
    for (i = 0; i < 3; i++){
        pts[i].setX(pts[i].x()+dx);
        pts[i].setY(pts[i].y()+dy);
    }

    m_vbo->bind();

    m_vbo->write(0, pts, 3*sizeof(vec2));
}

void Triangle::draw(QGLShaderProgram* prog){
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
   glDrawArrays(GL_TRIANGLES, 0, 3);

   m_vbo->release();

}

 bool Triangle::contains(const vec2 &pt){ // check
     if(leftOf(pts[0],pts[1], pt) && leftOf(pts[1],pts[2],pt)
             && leftOf(pts[2], pts[0], pt)){
             return true;
 }
     else{ return false;}

 }

