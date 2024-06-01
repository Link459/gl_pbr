#ersion 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

out vec3 color;
out vec3 normal;
out vec3 frag_pos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	frag_pos = vec3(transform * vec4(pos,1.0));
	normal = mat3(transpose(inverse(transform))) * in_normal;  
	//normal = in_normal;
    color = in_color;

    gl_Position = projection * view * transform * vec4(pos, 1.0);
}
