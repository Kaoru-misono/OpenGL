#version 330 core		
layout(location = 0) in vec3 a_Position;
//layout(location = 1) in vec3 a_Color;
layout(location = 1) in vec2 a_texCoord;

out vec3 v_Position;
//out vec3 v_Color;
out vec2 v_texCoord;

//uniform float u_Offset;
uniform mat4 u_mvp;

void main()
{
	v_Position = a_Position;
	//v_Color = a_Color;
	v_texCoord = a_texCoord;
	//v_Position.x += u_Offset;
	gl_Position = u_mvp * vec4(a_Position, 1.0);
}