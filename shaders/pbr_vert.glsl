#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

//out vec3 color;
out vec3 world_pos;
out vec3 normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
//uniform mat3 normal_matrix;

void main()
{
    world_pos = vec3(model * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(transform))) * in_normal;  
    color = in_color;

    gl_Position = projection * view * transform * vec4(pos, 1.0);
}

/*#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}*/
