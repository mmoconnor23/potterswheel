#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include <drawable.h>
namespace cs40{

class Rectangle : public Drawable {

   public:
     //Construct a new Rectangle with given corners
		 //vertices will be ordered CCW
     Rectangle(const vec2& lleft, const vec2& uright);
     
     Rectangle(const Rectangle* other);

     //No destructor necessary
     ~Rectangle() {/*do nothing*/};
     
     //Draw the Rectangle using openGL
     void draw(QGLShaderProgram* prog);

     void move(float dx, float dy);

     bool contains(const vec2 &pt);


   
   private:
     //Vertices of Rectangle in ccw order
     vec2 pts[4];
};

}//namespace

#endif //RECTANGLE_H
