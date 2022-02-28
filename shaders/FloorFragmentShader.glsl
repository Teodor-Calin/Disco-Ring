#version 330

// Input
in vec3 vertex_position_world_space;
in vec3 vertex_normal_world_space;

// Output
layout(location = 0) out vec4 out_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform float material_ke;
uniform int material_shininess;
uniform vec3 object_color;
uniform vec3 light_color;

uniform vec3 spotlights_pos[4];
uniform vec3 spotlights_col[4];
uniform float spotlights_angle;
uniform vec3 globe_pos;
uniform sampler2D globe_texture;
uniform float CurrentTime;
uniform int light_mode;

void main()
{
    vec3 color = vec3(0);
	float light = 0;
	float light_intensity = 7;

	// culoare proprie
	if (light_mode == 0 || light_mode == 1) {
		color += material_ke * object_color;
	}
	
	//spot-uri
	if (light_mode == 0 || light_mode == 2) {
		for (int i = 0; i < 4; i++) {

		vec3 light_direction = vec3(0, -1, 0);
	    vec3 viz_world_space = normalize(eye_position - vertex_position_world_space );
	    vec3 median_world_space = normalize (light_direction + viz_world_space);

		vec3 N = normalize(vertex_normal_world_space);
		vec3 L = normalize(spotlights_pos[i] - vertex_position_world_space);
		vec3 V = normalize(eye_position - vertex_position_world_space);
		vec3 H = normalize(L + V);

		float diffuse_light = material_kd * max(dot(N, L), 0) * light_intensity;
		float specular_light = 0;

		if (diffuse_light > 0)
			{
    			specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess)*light_intensity;
			}

	    float cut_off_rad = radians(spotlights_angle);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off_rad);
		
		if (spot_light > spot_light_limit) {
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor	= pow(linear_att, 2);
			light = light_att_factor * (diffuse_light + specular_light);
		} else {
			light = 0;
		}

		color += light * spotlights_col[i];
    }
	}

	// glob
	if (light_mode == 0 || light_mode == 3) {

		vec3 light_dir = vertex_position_world_space - globe_pos;
		vec2 texcoord;
		texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z);
		texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
		vec3 color1 = texture (globe_texture, texcoord + vec2(CurrentTime * 0.1, 0)).xyz;

		color += color1 * 0.2;
	}



    out_color = vec4(color, 1);
}
