#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

out vec2 tex_coords;
out vec3 normal;
out vec3 world_pos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	normal = mat3(transpose(inverse(transform))) * in_normal;  
    world_pos = vec3(transform * vec4(pos,1.0));
    tex_coords = in_tex_coords;
	//normal =   in_normal;  

    gl_Position = projection * view * transform * vec4(pos, 1.0);
}
