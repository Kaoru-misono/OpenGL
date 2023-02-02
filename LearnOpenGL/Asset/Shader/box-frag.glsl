#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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
in vec2 v_texcoord;
in vec3 v_world_pos;

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main()
{
	
	vec3 ambient_color = light.ambient * texture(material.diffuse, v_texcoord).rgb;
	vec3 emission_color = texture(material.emission, v_texcoord * 0.4).rgb;

	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(light.position - v_world_pos);
	vec3 diffuse_color = light.diffuse * max(0.0, dot(normal, light_dir)) * texture(material.diffuse, v_texcoord).rgb;
	
	vec3 view_dir = normalize(view_pos - v_world_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec3 specular_color = light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * texture(material.specular, v_texcoord).rgb;
	
	

	vec3 color = ambient_color + diffuse_color + specular_color + emission_color;
	frag_color = vec4(color, 1.0);
}