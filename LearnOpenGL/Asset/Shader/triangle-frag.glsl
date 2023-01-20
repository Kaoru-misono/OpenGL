#version 330 core
layout(location = 0) out vec4 FragColor;
			
in vec3 v_Position;
in vec3 v_Color;
in vec2 v_texCoord;
uniform vec4 u_Color;
uniform float u_time_factor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = u_Color;
	//FragColor = vec4(v_Color * u_time_factor, 1);
	//FragColor = texture(texture1, v_texCoord);
	FragColor = mix(texture(texture1, v_texCoord), texture(texture2, v_texCoord), 0.5);
}