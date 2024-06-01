#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

out vec3 local_pos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	local_pos = pos;
    gl_Position = projection * view * vec4(pos, 1.0);
}
