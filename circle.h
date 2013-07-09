#ifndef CIRCLE_H
#define CIRCLE_H

#include <drawable.h>
namespace cs40{

class Circle : public Drawable {

   public:
     //Construct a new Circle with given center and radius
     Circle(const vec2& center, float radius);

     Circle(const Circle* other);

     //No destructor necessary
     ~Circle() {/*do nothing*/};

     //Draw the Circle using openGL
     void draw(QGLShaderProgram* prog);

     void move(float dx, float dy);

     bool contains(const vec2 &pt);

   private:
     int sides;
     vec2 pts[303]; //+1 because of center
};

}//namespace

#endif //CIRCLE_H
