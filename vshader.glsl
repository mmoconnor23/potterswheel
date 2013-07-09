#version 130

uniform mat4 modelView;
in vec4 vPosition;

void main()
{
    vec4 pos = vPosition;
    pos.w=1.;
    gl_Position = modelView*pos;
}
