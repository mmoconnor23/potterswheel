#include "rectangle.h"
#include <iostream>
#include "geomfun.h"

using namespace cs40;

Rectangle::Rectangle(const vec2& lleft, const vec2& uright){
   /* pts[0] = lleft;
    pts[1] = vec2(uright.x(), lleft.y());
    pts[2] = vec2(lleft.x(), uright.y());
    pts[3] = uright;*/

    // we accept lines as rectangles, because we'd have
    // to define a standard rectangle otherwise
    if(lleft.x() <= uright.x() && lleft.y() <= uright.y()){
         pts[0] = lleft;
         pts[1] = vec2(uright.x(), lleft.y());
         pts[2] = vec2(lleft.x(), uright.y());
         pts[3] = uright;
    }


    else if(lleft.y() > uright.y() && lleft.x() < uright.x()){
        const vec2& correct_lleft =  vec2(lleft.x(),uright.y());
        const vec2& correct_uright = vec2(uright.x(),lleft.y());
        pts[0] = correct_lleft;
        pts[1] = uright;
        pts[2] = lleft;
        pts[3] = correct_uright;
     }

    else if(lleft.x() > uright.x() && lleft.y() < uright.y()){
        const vec2& correct_uright = vec2(lleft.x(),uright.y());
        const vec2& correct_lleft = vec2(uright.x(),lleft.y());
        pts[0] = correct_lleft;
        pts[1] = lleft;
        pts[2] = uright;
        pts[3] = correct_uright;

    }

    else {
        const vec2& correct_uright = lleft;
        const vec2& correct_lleft = uright;
        pts[0] = correct_lleft;
        pts[1] = vec2(correct_uright.x(), correct_lleft.y());
        pts[2] = vec2(correct_lleft.x(), correct_uright.y());
        pts[3] = correct_uright;

    }



    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,4*sizeof(vec2));

    m_visible = true;
}

Rectangle::Rectangle(const Rectangle *other){
  /* implement copy constructor */
    pts[0] = other->pts[0];
    pts[1] = other->pts[1];
    pts[2] = other->pts[2];
    pts[3] = other->pts[3];

    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,4*sizeof(vec2));

    m_visible = true;
}

void Rectangle::move(float dx, float dy){
  /* implement this.
   * update pts
   * write updates to old QGLBuffer
   */
    int i;
    for (i = 0; i < 4; i++){
        pts[i].setX(pts[i].x()+dx);
        pts[i].setY(pts[i].y()+dy);
    }

    m_vbo->bind();

    m_vbo->write(0, pts, 4*sizeof(vec2));
}

void Rectangle::draw(QGLShaderProgram* prog){
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
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   m_vbo->release();





}

 bool Rectangle::contains(const vec2 &pt){
     if(pt.x() > pts[0].x() &&pt.x() < pts[1].x()
             && pt.y() > pts[0].y() && pt.y()< pts[3].y()){
         return true;
     }
     return false;

 }
