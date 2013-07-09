#include "circle.h"
#include <iostream>
#include <cmath>

const float pi = 3.14;
int param = 5;
int numpoints = 360/param;

using namespace cs40;

Circle::Circle(const vec2& center, float radius){
sides = 300;
pts[0] = center;
for (int t=1; t<sides+1; t++) {
pts[t] = vec2(center.x() + radius*cos((t-1)*2*M_PI/sides),
center.y() + radius*sin((t-1)*2*M_PI/sides));
}
//adding the first point again to get the last wedge
pts[sides+1] = pts[1];

initVBO();
m_vbo->bind();
m_vbo->allocate(pts, (sides+2)*sizeof(vec2));

m_visible = true;

}


Circle::Circle(const Circle *other){
  /* implement copy constructor */
    int i;
    for (i=0; i<numpoints+1; i++) {
        pts[i] = other->pts[i];
    }

    if(!initVBO()){
        std::cout<<"Could not create vbo"<<endl;
    }

    m_vbo->bind();

    m_vbo->allocate(pts,(numpoints+1)*sizeof(vec2));

    m_visible = true;
}

void Circle::move(float dx, float dy){
/*
* update pts
* write updates to old QGLBuffer
*/

for (int i=0; i < sides+2; i++) {
pts[i] += vec2(dx, dy);
}

m_vbo->bind();
m_vbo->write(0, pts, (sides+2)*sizeof(vec2));

}
void Circle::draw(QGLShaderProgram* prog){
    /* bind active vbo
     * set color uniform value in fragment shader
     * enable attribute array for vPosition input in vertex shader
     * describe layout of VBO for attribute array
     * glDrawArrays
     * release vbo
     */


   m_vbo->bind();
   prog->setUniformValue("vColor", m_color);
   prog->enableAttributeArray("vPosition");
   prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);
   glDrawArrays(GL_TRIANGLE_FAN, 0, sides+2);
   m_vbo->release();





}


bool Circle::contains(const vec2 &pt){
    vec2 radVec = pts[1]- pts[0];
    float radius = radVec.length();

    vec2  distVec = pt - pts[0];
    float dist = distVec.length();

    if(dist<= radius){
        return true;
    }
    return false;

}
