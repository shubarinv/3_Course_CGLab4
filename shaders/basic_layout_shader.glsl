#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;// the position variable has attribute position 0
layout (location = 1) in vec3 aColor;// the color variable has attribute position 1

out vec3 ourColor;// output a color to the fragment shader
uniform mat4 u_MVP;
void main()
{
    gl_Position = u_MVP*vec4(aPos, 1.0);
    ourColor = aColor;// set ourColor to the input color we got from the vertex data
}

    #shader fragment
    #version 410 core
out vec4 FragColor;
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}