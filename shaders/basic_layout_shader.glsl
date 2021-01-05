#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

    #shader fragment
    #version 410 core
out vec4 FragColor;
void main()
{
   FragColor = vec4(1.0f, 0.1f, 0.2f, 1.0f);
}