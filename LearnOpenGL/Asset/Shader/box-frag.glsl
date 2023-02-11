#version 330 core

#define POINT_LIGHTS_NUM 4

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
uniform Point_Light p_lights[POINT_LIGHTS_NUM];
uniform Spot_Light s_light;
uniform vec3 view_pos;

vec3 calculate_dir_light(Direcitonal_Light light, vec3 normal, vec3 view_dir)
{
	vec3 ambient_color = light.ambient * texture(material.diffuse, v_texcoord).rgb;

	vec3 light_dir = normalize(-light.direction);
	vec3 diffuse_color = light.diffuse * max(0.0, dot(normal, light_dir)) * texture(material.diffuse, v_texcoord).rgb;

	vec3 reflect_dir = reflect(-light_dir, normal);
	vec3 specular_color = light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * texture(material.specular, v_texcoord).rgb;
	return ambient_color + diffuse_color + specular_color;
}

vec3 calculate_point_light(Point_Light light, vec3 normal, vec3 world_pos, vec3 view_dir)
{
	float Kc = light.attenuation_factor.x, Kl = light.attenuation_factor.y, Kq = light.attenuation_factor.z;
	float r_light_to_obj = length(light.position - world_pos);
	float attenuation = 1.0/(Kc + Kl * r_light_to_obj + Kq * (r_light_to_obj * r_light_to_obj));
	
	vec3 ambient_color = light.ambient * texture(material.diffuse, v_texcoord).rgb;
	ambient_color *= attenuation;

	vec3 light_dir = normalize(light.position - world_pos);
	vec3 diffuse_color = light.diffuse * max(0.0, dot(normal, light_dir)) * texture(material.diffuse, v_texcoord).rgb;
	diffuse_color *= attenuation;

	vec3 reflect_dir = reflect(-light_dir, normal);
	vec3 specular_color = light.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * texture(material.specular, v_texcoord).rgb;
	specular_color *= attenuation;

	return ambient_color + diffuse_color + specular_color;
}

vec3 calculate_spot_light(Spot_Light light, vec3 normal, vec3 world_pos, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - world_pos);
	float cos_theta = dot(light_dir, normalize(-light.direction));
	float intensity = clamp((light.in_cut_off - cos_theta )/(light.out_cut_off - light.in_cut_off), 0.0, 1.0);
	
	vec3 ambient_color = light.ambient * texture(material.diffuse, v_texcoord).xyz;

	vec3 diffuse_color = intensity * light.diffuse * texture(material.diffuse, v_texcoord).xyz * max(0.0, dot(normal, light_dir));

	vec3 half_vector = normalize(view_dir + light_dir);

	vec3 specular_color = intensity * light.specular * texture(material.specular, v_texcoord).xyz
	* pow(max(0.0, dot(normal, half_vector)), material.shininess);
	return ambient_color + diffuse_color + specular_color;
}

void main()
{
	vec3 normal = normalize(v_normal);
	vec3 view_dir = normalize(view_pos - v_world_pos);	
	
	vec3 color = vec3(0.0);
	color += calculate_dir_light(d_light, normal, view_dir);
	//for(int i = 0; i < POINT_LIGHTS_NUM; i++)
	//color += calculate_point_light(p_lights[i], normal, v_world_pos, view_dir);
	color += calculate_spot_light(s_light, normal, v_world_pos, view_dir);
	//if (emission_color != vec3(0.0)) color = emission_color;
	frag_color = vec4(color, 1.0);
}