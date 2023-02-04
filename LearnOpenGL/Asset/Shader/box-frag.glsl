#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Direcitonal_Light
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point_Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation_factor;
};

out vec4 frag_color;

in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_world_pos;

uniform Material material;
uniform Direcitonal_Light d_light;
uniform Point_Light p_light;
uniform vec3 view_pos;

void main()
{
	float Kc = p_light.attenuation_factor.x, Kl = p_light.attenuation_factor.y, Kq = p_light.attenuation_factor.z;
	float r_light_to_obj = length(p_light.position - v_world_pos);
	float attenuation = 1.0/(Kc + Kl * r_light_to_obj + Kq * (r_light_to_obj * r_light_to_obj));
	
	vec3 ambient_color = p_light.ambient * texture(material.diffuse, v_texcoord).rgb;
	ambient_color *= attenuation;
	vec3 emission_color = texture(material.emission, v_texcoord * 0.4).rgb;

	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(-d_light.direction);
	vec3 diffuse_color = p_light.diffuse * max(0.0, dot(normal, light_dir)) * texture(material.diffuse, v_texcoord).rgb;
	diffuse_color *= attenuation;

	vec3 view_dir = normalize(view_pos - v_world_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec3 specular_color = p_light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * texture(material.specular, v_texcoord).rgb;
	specular_color *= attenuation;
	

	vec3 color = ambient_color + diffuse_color + specular_color;
	//if (emission_color != vec3(0.0)) color = emission_color;
	frag_color = vec4(color, 1.0);
}