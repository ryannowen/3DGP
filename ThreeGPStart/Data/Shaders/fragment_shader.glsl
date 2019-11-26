#version 330

struct LightData
{
	// Types
	// 0 = Directional
	// 1 = Point
	// 2 = Spot

	int light_type; 
	vec3 light_position;
	vec3 light_direction;
	float light_fov;
	vec3 light_colour;
	float light_range;
	float light_intensity;
};

in vec3 varying_position;
in vec2 varying_uv;
in vec3 varying_normal;

out vec4 fragment_colour;

uniform mat4 model_xform;
uniform sampler2D texSample;
uniform vec3 camera_direction;
uniform int numOfLights;
uniform LightData lights[3];

void main(void)
{
	vec3 ambient_colour = vec3(0.05);
	float specular_intensity = 60;
	vec4 diffuse_colour = texture(texSample, varying_uv);

	vec3 N = normalize(varying_normal);
	vec3 P = varying_position;


	fragment_colour += vec4(ambient_colour * vec3(1), 1);

	for(int i = 0; i < numOfLights; i++)
	{
		vec3 L;
		float attenuation = 1.0;

		if(lights[i].light_type != 0) // Not a Directional Light
		{
			L = normalize(lights[i].light_position - P);

			// Attenuation
			float light_distance = distance(lights[i].light_position, P);
			attenuation = 1.0 - smoothstep(0, lights[i].light_range, light_distance);
		}
		else // Directional Light
		{
			L = normalize(-lights[i].light_direction);
		}


		// Diffuse
		vec3 diffuse_intensity = max(vec3(0.0), dot(L, N));
		diffuse_intensity *= max(vec3(0.1), lights[i].light_colour) * max(0.01, lights[i].light_intensity);

		// Specular
		vec3 rV = reflect(L, N);
		float LR = max(0, dot(camera_direction, rV));
		vec3 specular = diffuse_colour.rgb * pow(LR, specular_intensity);

		if(lights[i].light_type == 2) // Spot light
		{
			attenuation *= smoothstep(cos(0.5 * lights[i].light_fov), 1, dot(L, -lights[i].light_direction));
		}


		// Final Calculation
		fragment_colour += vec4(((diffuse_colour.rgb + specular) * diffuse_intensity * attenuation), 0.0);

		//fragment_colour = vec4(varying_normal, 1.0);
	}
}