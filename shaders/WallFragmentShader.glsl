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

uniform vec3 floor_light_pos[3];
uniform vec3 floor_light_colors[3];
uniform vec3 globe_pos;
uniform sampler2D globe_texture;
uniform float CurrentTime;
uniform int light_mode;

void main()
{
    float light_radius = 2;
    float light;

    vec3 color = vec3(0);

    // podea
    if (light_mode == 0 || light_mode == 1) {
        for (int i = 0; i < 3; i++) {

	        vec3 light_direction = normalize(floor_light_pos[i] - vertex_position_world_space);
	        vec3 viz_world_space = normalize(eye_position - vertex_position_world_space );
	        vec3 median_world_space = normalize (light_direction + viz_world_space);

            // diffuse light
            float light_intensity = 1.0;
	        float diffuse_light = material_kd * light_intensity * max(dot(vertex_normal_world_space, light_direction), 0);

            // specular light
            float specular_light;

            if (diffuse_light > 0)
	        {
	            //Phong
	            vec3 R = reflect(-light_direction, vertex_normal_world_space);
	            specular_light = material_ks * light_intensity * pow(max(dot(viz_world_space, R), 0), material_shininess);
	        }

            // Compute light
            float d = distance(floor_light_pos[i], vertex_position_world_space);

            float aFactor = 0;
            if (d < light_radius) {
                 aFactor = pow(light_radius - d, 2);
            }

            light = aFactor * (diffuse_light + specular_light);
            color += light * floor_light_colors[i];
    }
    }

    // glob
    if (light_mode == 0 || light_mode == 3) {
        vec3 light_dir = vertex_position_world_space - globe_pos;
        vec2 texcoord;
        texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z);
        texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
        vec3 color1 = texture (globe_texture, texcoord + vec2(CurrentTime * 0.1, 0)).xyz;

        color += color1 * 0.3;
    }


    out_color = vec4(color, 1);
}
