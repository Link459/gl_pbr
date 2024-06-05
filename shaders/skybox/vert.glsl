#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coords;

uniform mat4 projection;
uniform mat4 view;

out vec3 local_pos;

void main()
{
    local_pos = aPos;

    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = projection * rotView * vec4(local_pos, 1.0);

    gl_Position = clipPos.xyww;
}
