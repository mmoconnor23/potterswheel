#ifndef GEOMFUN_H
#define GEOMFUN_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

namespace cs40{


  /* return true if p1 is left of line from p2 to p3 */
  bool leftOf(const vec2& p1, const vec2& p2, const vec2& p3);
  float distToSegment(const vec2& clickPt, const vec2& p1, const vec2& p2);

} //namespace


#endif // GEOMFUN_H
