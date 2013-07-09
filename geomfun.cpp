#include "geomfun.h"
#include <iostream>

using namespace cs40;

bool cs40::leftOf(const vec2& p1, const vec2& p2, const vec2& p3){
    vec2 v = p2 - p1;
    vec2 w = p3 - p1;
    vec3 result = QVector3D::crossProduct(v,w);
    if(result.z() > 0){
        return true;
    }
    return false;

}

float cs40::distToSegment(const vec2& p1, const vec2& p2, const vec2& p3){
    vec2 u = p3 - p2;
    vec2 v = p1 - p2;


    vec3 dist = QVector3D::crossProduct(u,v)/u.length();
    return dist.length();


}



