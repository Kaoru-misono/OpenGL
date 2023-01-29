#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out vec3 v_normal;
out vec3 v_world_pos;

uniform mat4 u_model;
uniform mat4 u_normal_matrix;
uniform mat4 u_mvp;

void main()
{
	v_normal = mat3(u_normal_matrix) * a_normal;
	gl_Position = u_mvp * vec4(a_position, 1.0);
	v_world_pos = vec3(u_model * vec4(a_position, 1.0));
}

