#include "camera.h"

using namespace cs40;

Camera::Camera(){
    /* Construct a new camera
     * with eye at origin
     * looking down -z axis
     * up direction in +y axis
     */
    //identity matrix
    m_lookAt = mat4();
}

void Camera::lookAt(const vec3& eye, const vec3& at, const vec3& up){
    /* Reproduce behavior of QMatrix4x4::lookAt */
    //translate camera to eye position
    vec3  n_hat, alpha_n, vo;

    n = (at - eye)/(at - eye).length();
    n_hat = n/n.length();

    alpha_n = QVector3D::dotProduct(up, n_hat)*n_hat;

    vo = up - (alpha_n);
    v = vo/vo.length();
    u = QVector3D::crossProduct(n_hat, v);

    m_lookAt = mat4(u.x(), u.y(), u.z(), -1*QVector3D::dotProduct(u, eye),
                    v.x(), v.y(), v.z(), -1*QVector3D::dotProduct(v, eye),
                    -1*n.x(), -1*n.y(), -1*n.z(), QVector3D::dotProduct(n, eye),
                    0, 0, 0, 1);
}

void Camera::right(float amt){
    /* move camera right by amount amt. Scene should shift left
     * amt is in world units */
    m_lookAt.translate(amt, 0, 0);
}

void Camera::up(float amt){
    /* move camera up */
    m_lookAt.translate(0, -amt, 0);
}

void Camera::forward(float amt){
    /* move camera forward */
    m_lookAt.translate(0, 0, -amt);
}


void Camera::roll(float angle){
    /* rotate CCW about -z axis _of camera_
     * angle is in degrees */
    m_lookAt.rotate(angle,n);
}

void Camera::pitch(float angle){
    /* rotate CCW about +x */
    m_lookAt.rotate(angle,u);
}

void Camera::yaw(float angle){
    /* CCW about +y */
    m_lookAt.rotate(angle,v);
}

void Camera::reset(){
    /* reset camera to default location
     * as in constructor */

    lookAt(vec3(0, 0, -20000), vec3(0,0,0), vec3(0,1,0));
}
