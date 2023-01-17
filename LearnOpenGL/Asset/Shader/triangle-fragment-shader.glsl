#version 330 core
layout(location = 0) out vec4 FragColor;
			
in vec3 v_Position;
in vec3 v_Color;
uniform vec4 u_Color;
uniform float u_time_factor;

void main()
{
	//FragColor = u_Color;
	FragColor = vec4(v_Color * u_time_factor, 1);
}