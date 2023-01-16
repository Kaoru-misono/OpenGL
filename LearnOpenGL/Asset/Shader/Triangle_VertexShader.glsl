#version 330 core		
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

out vec3 v_Position;
out vec3 v_Color;

//uniform float u_Offset;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	//v_Position.x += u_Offset;
	gl_Position = vec4(v_Position, 1.0);
}