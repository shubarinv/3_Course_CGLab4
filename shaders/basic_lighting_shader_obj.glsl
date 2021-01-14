#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;
layout(location=2)in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    v_TexCoord =texCoord;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

    #shader fragment
    #version 410 core

out vec4 FragColor;
layout(location=0)out vec4 color;

in vec2 v_TexCoord;
uniform sampler2D u_Texture;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) ;
    FragColor = vec4(result, 1.0);
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = FragColor*texColor;
}