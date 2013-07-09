#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <drawable.h>
namespace cs40{

class Triangle : public Drawable {

   public:
     /* Construct a new triangle with given endpoints 
			* Triangle should be oriented CCW */
     Triangle(const vec2& p1, const vec2& p2, const vec2& p3);
     
     Triangle(const Triangle* other);

     //No destructor necessary
     ~Triangle() {/*do nothing*/};
     
     //Draw the Triangle using openGL
     void draw(QGLShaderProgram* prog);

     void move(float dx, float dy);
  
		 /* TODO: implement a real version in triangle.cpp */ 
     bool contains(const vec2 &pt);

   private:
     //Endpoints of Triangle in ccw order
     vec2 pts[3];
};

}//namespace

#endif //TRIANGLE_H
