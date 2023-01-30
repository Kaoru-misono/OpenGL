#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 frag_color;

in vec3 v_normal;
in vec3 v_world_pos;

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main()
{
	
	vec3 ambient = light.ambient * material.ambient;

	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(light.position - v_world_pos);
	vec3 diffuse_color = light.diffuse * max(0.0, dot(normal, light_dir)) * material.diffuse;
	
	vec3 view_dir = normalize(view_pos - v_world_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec3 specular_color = light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * material.specular;
	
	

	vec3 color = ambient + diffuse_color + specular_color;
	frag_color = vec4(color, 1.0);
}