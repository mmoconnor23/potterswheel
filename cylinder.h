#ifndef _CYLINDER_H
#define _CYLINDER_H

#include <QGLBuffer>
#include <QGLShaderProgram>
#include <cmath>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40{

class Cylinder {
public:
    /* construct a Cylinder centered at origin with given radius
         * and height (along +y axis)
         * by dividing it into vertical slices and horizontal stacks */
    Cylinder( int slices, int stacks, QList<QVector2D> pts);

    ~Cylinder();

    /* draw the sphere using provided shader program  */
    void draw(QGLShaderProgram* prog);

    /* Set Ambient and Diffuse color of Cylinder */
    inline void setColor(const vec3& color){ m_color=color; m_color.setW(1.); }

    inline void setSpecularColor(const vec3& color){
            m_spec_color=color; m_spec_color.setW(1.); }

    /* Get current color of object */
    inline vec4 getAmbientAndDiffuseColor() const { return m_color; }

    inline vec4 getSpecularColor() const {return m_spec_color;}

private:

    /* Generate Vertex Buffer Objects, but do not bind/allocate.
     * Return true if generate was a success, false otherwise
     */
    bool initVBO();


protected:
    vec4 m_color;
    vec4 m_spec_color;
    QGLBuffer *m_vbo;
    float m_radius0;
    float m_radius1;
    float m_height;
    int m_slices;
    int m_stacks;
    int m_stripsize; //num vertices per horizontal strip;
};

} //namespace

#endif //CYLINDER_H
