#ifndef _SQUARE_H
#define _SQUARE_H

#include <QtOpenGL>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40{

class Square {
public:
    /* construct a square with given width
		 * centered at origin, in z=0 plane*/
    Square(float width);

    ~Square();

    /* draw the square using provided shader program  */
    void draw(QGLShaderProgram* prog);

    /* Set color of Square */
    inline void setColor(const vec3& color){ m_color=color; m_color.setW(1.); }

    /* Get current color of object */
    inline vec4 getColor() const { return m_color; }

private:

    /* Generate Vertex Buffer Objects, but do not bind/allocate.
     * Return true if generate was a success, false otherwise
     */
    bool initVBO();


protected:
    vec4 m_color;
    QGLBuffer *m_vbo;
    float m_width;
};

} //namespace

#endif //SQUARE_H
