#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float state;

out vec3 ourColor;

void main()
{
    if (state <10) {
        gl_PointSize = 5.0;
    } else if(state <20) {
        gl_PointSize = 3.0;
    } else {
        gl_PointSize = 2.0;
    }
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}