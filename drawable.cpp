#include "drawable.h"
#include <iostream>

using namespace cs40;

bool Drawable::initVBO(){
    m_vbo = new QGLBuffer(QGLBuffer::VertexBuffer);
    bool ok = m_vbo->create();
    m_vbo->setUsagePattern(QGLBuffer::DynamicDraw);
    return ok;
}

Drawable::~Drawable(){
    if(m_vbo){
        m_vbo->release();
        delete m_vbo; m_vbo=NULL;
    }
}
