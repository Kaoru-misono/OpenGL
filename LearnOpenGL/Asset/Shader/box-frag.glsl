#version 330 core

out vec4 frag_color;

in vec3 v_normal;
in vec3 v_world_pos;

uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(light_pos - v_world_pos);
	vec3 view_dir = normalize(view_pos - v_world_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float specular_strength = 0.5;
	vec3 specular_color = specular_strength * light_color * pow(max(dot(view_dir, reflect_dir), 0.0), 128);
	vec3 diffuse_color = light_color * max(0.0, dot(normal, light_dir));

	vec3 color = (ambient + diffuse_color + specular_color) * object_color;
	frag_color = vec4(color, 1.0);
}