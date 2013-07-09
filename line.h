#ifndef LINE_H
#define LINE_H

#include <drawable.h>
namespace cs40{

class Line : public Drawable {

   public:


     //Construct a new Line with given endpoints
         //vertices will be ordered CCW
     Line(const vec2& pt1, const vec2& pt2);

     Line(const Line* other);

     //No destructor necessary
     ~Line() {/*do nothing*/};

     //Draw the Line using openGL
     void draw(QGLShaderProgram* prog);

     void move(float dx, float dy);

     bool contains(const vec2 &pt);

     bool isVisible();

   private:
     //Endpoints of Line in ccw order
     vec2 pts[2];
     bool visible;

};

}//namespace

#endif // LINE_H
