#shader vertex
#version 410 core

layout (std140) uniform Matrices {

    mat4 projMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    Normal = normalize(vec3(viewMatrix * modelMatrix * vec4(normal,0.0)));
    TexCoord = vec2(texCoord);
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position,1.0);
}

    #shader fragment
    #version 410 core


layout (std140) uniform Material {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float shininess;
    int texCount;
};

uniform	sampler2D texUnit;

in vec3 Normal;
in vec2 TexCoord;
out vec4 outpt;

void main()
{
    vec4 color;
    vec4 amb;
    float intensity;
    vec3 lightDir;
    vec3 n;

    lightDir = normalize(vec3(1.0,1.0,1.0));
    n = normalize(Normal);
    intensity = max(dot(lightDir,n),0.0);

    if (texCount == 0) {
        color = diffuse;
        amb = ambient;
    }
    else {
        color = texture2D(texUnit, TexCoord);
        amb = color * 0.33;
    }
    outpt = (color * intensity) + amb;
}
