#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <QMatrix4x4>

namespace cs40{

class MatrixStack
{
public:
    /* construct a stack containing identity matrix */
    MatrixStack();

    QMatrix4x4& top();

    /* push copy of top onto stack */
    void push();

    /* pop top element off stack */
    void pop();

    /* translate matrix at top of stack in place
     * all matrix tranforms multiply on the right
     * of top */
    void translate(float dx, float dy, float dz);

    /* scales matrix at top of stack in place */
    void scale(float fac);

private:
    QList<QMatrix4x4> m_stack;
};

} // namespace

#endif // MATRIXSTACK_H
