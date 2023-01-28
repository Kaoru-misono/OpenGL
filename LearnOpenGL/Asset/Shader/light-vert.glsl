#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * vec4(a_position, 1.0);
}

