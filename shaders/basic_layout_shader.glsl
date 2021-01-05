#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;// the position variable has attribute position 0


void main()
{
    gl_Position =vec4(aPos, 1.0);
}

    #shader fragment
    #version 410 core
out vec4 FragColor;


void main()
{
    FragColor = vec4(0.7, 0, 0.5, 1.0);
}