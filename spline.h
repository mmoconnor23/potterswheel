#ifndef SPLINE_H
#define SPLINE_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QGLShaderProgram>
#include <QGLBuffer>

typedef QVector2D vec2;
typedef QVector4D vec4;

class Spline
{
public:
    Spline(QList<vec2> array, int iteration_num ,int length);

    void draw(QGLShaderProgram* prog, int iteration_num, int space_allocation);

    bool created;

protected:

QGLBuffer *pt_vbo;
vec4 m_color;

};




#endif // SPLINE_H
