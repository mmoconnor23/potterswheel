#ifndef _DRAWABLE_H
#define _DRAWABLE_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QGLShaderProgram>
#include <QGLBuffer>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40{

/* not much going on here. Functionality in derived classes */
class Drawable {
  public:
    Drawable(): m_vbo(NULL) {}; //default, empty constructor


    /* classes can have only a single destructor "virtual" with a destructor
     * means that derived classes can change the functionality of the base
     * class destructor by adding their own destructor to clean-up their part
     * of the class. Without virtual only the base class destructor is invoked.
     * Overriding a destructor has different semantics than overriding
     * a regular function (all destructors are invoked). Destructors are called
     * in this order:
     *   1. derived class destructor 2. base class destructor
     */
    virtual ~Drawable();

    /* Pure virtual function. Must be implemented by derived class
     *  prog: reference to shader program. Assumse a vec4
     *        uniform color for fragment shader*/
    virtual void draw(QGLShaderProgram* prog)=0;

    /* Generate Vertex Buffer Object, but do not bind/allocate.
     * Return true if generate was a success, false otherwise
     */
    bool initVBO();

    /* Set color of object */
    inline void setColor(const vec3& color){ m_color=color; m_color.setW(1.); }

    /* Move the object by an amount dx, dy
     * Pure virtual. Must be implemented in derived class
     */
    virtual void move(float dx, float dy) = 0;

    /* return true if shape contains point pt, false otherwise
     * Pure virtual. Must be implemented in derived class
     */
    virtual bool contains(const vec2& pt) = 0;

    /* Get current color of object */
    inline vec4 getColor() const { return m_color; }

    /* Hide shape so that it is not drawn */
    inline void hide(){ m_visible=false; }

    /* return true if visibile, false otherwise */
    inline bool isVisible(){ return m_visible; }

    protected:
        vec4 m_color;
        QGLBuffer *m_vbo;
        bool m_visible;

};

} //namespace

#endif //DRAWABLE_H
