#include "spline.h"
#include <iostream>

Spline::Spline(QList<vec2> array, int iteration_num, int length)
{

    pt_vbo = new QGLBuffer(QGLBuffer::VertexBuffer);
    bool ok = pt_vbo->create();
    if(!ok){
        std::cout << "Could not create VBO" <<std::endl;
    }
    pt_vbo->setUsagePattern(QGLBuffer::DynamicDraw);

    vec2* pts_array = new vec2[length];

    for (int i = 0; i < array.length(); i++){
        pts_array[i] = array.at(i);
    }


    pt_vbo->bind();
    pt_vbo->allocate(pts_array, length*sizeof(vec2));

    created = true;
    m_color = vec4(1., 0, 0,1.);
}


 void Spline::draw(QGLShaderProgram* prog, int iteration_num, int space_allocation){


       pt_vbo->bind();
       prog->setUniformValue("color", m_color);
       prog->enableAttributeArray("vPosition");
       prog->setAttributeBuffer("vPosition", GL_FLOAT, 0, 2, 0);
       glDrawArrays(GL_LINE_STRIP, 0, 41 + iteration_num*20 + space_allocation);
       pt_vbo->release();

}
