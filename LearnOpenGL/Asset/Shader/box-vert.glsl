#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out vec3 v_normal;
out vec3 v_world_pos;
out vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_normal_matrix;
uniform mat4 u_mvp;

void main()
{
	v_normal = mat3(u_normal_matrix) * a_normal;
	v_texcoord = a_texcoord;
	gl_Position = u_mvp * vec4(a_position, 1.0);
	v_world_pos = vec3(u_model * vec4(a_position, 1.0));
}

