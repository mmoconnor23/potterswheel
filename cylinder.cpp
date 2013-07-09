#include "cylinder.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Cylinder::Cylinder( int slices, int stacks,  QList<QVector2D> pts):
    m_color(1.,0.,0.,0.), m_spec_color(1.,1.,1.,1.), m_vbo(NULL), m_slices(slices), m_stacks(stacks)
{

    m_stripsize = (slices+1)*2;
    int nverts = m_stripsize*(m_stacks)+2*(slices+2);
    vec3* vertices = new vec3[nverts];
    vec3* normals = new vec3[nverts];
    vec2* texCoords = new vec2[nverts];

    double longstep = 2.*M_PI/m_slices;

    double texXstep=1./m_slices;
    double texYstep=1./m_stacks;
    double y0 = pts.at(0).y();
    double texY = 0;

    double lng = M_PI;
    double texX = 0;
    double coslong = cos(lng);
    double sinlong = sin(lng);
    double coslong1= cos(lng) ;
    double sinlong1 = sin(lng);
    int idx = 0;

    /* mid strips */
    for(int i = 0; i < m_stacks -1; i++){

       double y1 = pts.at(i+1).y();

        lng = -M_PI;
        texX = 0;
        m_radius0 = pts.at(i).x();
        m_radius1 = pts.at(i+1).x();


        for(int j=0; j<=m_slices; j++){
            coslong = m_radius0*cos(lng);
            sinlong = m_radius0*sin(lng);
            coslong1 = m_radius1*cos(lng);
            sinlong1 = m_radius1*sin(lng);

            vertices[idx]=vec3(sinlong1, y1, coslong1);
            normals[idx]=vec3((y0/y1)*sin(lng),0,(y0/y1)*cos(lng)); //yo , we need to change this
            texCoords[idx++]=vec2(texX,texY+texYstep);
            vertices[idx]=vec3(sinlong, y0, coslong);
            normals[idx]=vec3((y0/y1)*sin(lng),0,(y0/y1)*cos(lng)); //yo , we need to change this
            texCoords[idx++]=vec2(texX,texY);
            lng+=longstep;
            texX+=texXstep;
        }

        y0 = pts.at(i).y();
        texY+=texYstep;
    }

    if(initVBO()){
        /* all mid lat strips */
        int DataSize = m_stripsize*(m_stacks)*sizeof(vec3);
        /* two polar fans*/
        DataSize += 2*(m_slices+2)*sizeof(vec3);

        /* size of Texture */
        /* all mid lat strips */
        int TexSize = m_stripsize*(m_stacks)*sizeof(vec2);
        /* two polar fans*/
        TexSize += 2*(m_slices+2)*sizeof(vec2);

        /* size of Normals */
        int NormalSize = DataSize;

        m_vbo->bind();
        m_vbo->allocate(DataSize+TexSize+NormalSize);
        m_vbo->write(0,vertices,DataSize);
        m_vbo->write(DataSize,texCoords,TexSize);
        m_vbo->write(DataSize+TexSize,normals,NormalSize);
        m_vbo->release();
    }

    delete [] vertices; vertices=NULL;
    delete [] texCoords; texCoords=NULL;
    delete [] normals; normals=NULL;
}

bool Cylinder::initVBO(){
    m_vbo = new QGLBuffer(QGLBuffer::VertexBuffer);
    bool ok = m_vbo->create();
    m_vbo->setUsagePattern(QGLBuffer::StaticDraw);
    return ok;
}

Cylinder::~Cylinder(){
    if(m_vbo){
        m_vbo->release();
        delete m_vbo; m_vbo=NULL;
    }
}

void Cylinder::draw(QGLShaderProgram* prog){
    if(!prog){ return; }
    m_vbo->bind();
    prog->setUniformValue("vColor",m_color);
    prog->setUniformValue("vSColor", m_spec_color);
    prog->enableAttributeArray("vPosition");
    prog->setAttributeBuffer("vPosition",GL_FLOAT,0,3,0);
    prog->enableAttributeArray("vTexture");
    int nverts = (m_stacks)*m_stripsize+2*(m_slices+2);
    prog->setAttributeBuffer("vTexture",GL_FLOAT,nverts*sizeof(vec3),2,0);
    prog->enableAttributeArray("vNormal");
    prog->setAttributeBuffer("vNormal",GL_FLOAT,
                             nverts*(sizeof(vec3)+sizeof(vec2)),3,0);
    for(int i=0; i<m_stacks; i++){
        glDrawArrays(GL_TRIANGLE_STRIP,i*m_stripsize, m_stripsize);
    }
    int offset = (m_stacks)*m_stripsize;
    int fansize = m_slices+2;
    glDrawArrays(GL_TRIANGLE_FAN, offset, fansize);
    glDrawArrays(GL_TRIANGLE_FAN, offset+fansize, fansize);
    m_vbo->release();
}
