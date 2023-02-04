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

struct Spot_Light
{
	vec3 position;
	vec3 direction;
	float in_cut_off;
	float out_cut_off;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

out vec4 frag_color;

in vec3 v_normal;
in vec2 v_texcoord;
in vec3 v_world_pos;

uniform Material material;
uniform Direcitonal_Light d_light;
uniform Point_Light p_light;
uniform Spot_Light s_light;
uniform vec3 view_pos;

void main()
{
//	float Kc = p_light.attenuation_factor.x, Kl = p_light.attenuation_factor.y, Kq = p_light.attenuation_factor.z;
//	float r_light_to_obj = length(p_light.position - v_world_pos);
//	float attenuation = 1.0/(Kc + Kl * r_light_to_obj + Kq * (r_light_to_obj * r_light_to_obj));
//	
//	vec3 ambient_color = p_light.ambient * texture(material.diffuse, v_texcoord).rgb;
//	ambient_color *= attenuation;
//	vec3 emission_color = texture(material.emission, v_texcoord * 0.4).rgb;
//
//	vec3 normal = normalize(v_normal);
//	vec3 light_dir = normalize(-d_light.direction);
//	vec3 diffuse_color = p_light.diffuse * max(0.0, dot(normal, light_dir)) * texture(material.diffuse, v_texcoord).rgb;
//	diffuse_color *= attenuation;
//
//	vec3 view_dir = normalize(view_pos - v_world_pos);
//	vec3 reflect_dir = reflect(-light_dir, normal);
//	vec3 specular_color = p_light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * texture(material.specular, v_texcoord).rgb;
//	specular_color *= attenuation;
	
	vec3 normal = normalize(v_normal);
	vec3 light_dir = normalize(s_light.position - v_world_pos);
	float cos_theta = dot(light_dir, normalize(-s_light.direction));
	float intensity = clamp((s_light.in_cut_off - cos_theta )/(s_light.out_cut_off - s_light.in_cut_off), 0.0, 1.0);

	vec3 color = vec3(1.0f);

	
	vec3 ambient_color = s_light.ambient * texture(material.diffuse, v_texcoord).xyz;

	vec3 diffuse_color = intensity * s_light.diffuse * texture(material.diffuse, v_texcoord).xyz * max(0.0, dot(normal, light_dir));

	vec3 view_dir = normalize(view_pos - v_world_pos);
	vec3 half_vector = normalize(view_dir + light_dir);

	vec3 specular_color = intensity * s_light.specular * texture(material.specular, v_texcoord).xyz
	* pow(max(0.0, dot(normal, half_vector)), material.shininess);
	color = ambient_color + diffuse_color + specular_color;

		
	
	//if (emission_color != vec3(0.0)) color = emission_color;
	frag_color = vec4(color, 1.0);
}