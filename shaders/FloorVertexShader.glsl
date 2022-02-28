#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output value to fragment shader
out vec3 vertex_position_world_space;
out vec3 vertex_normal_world_space;

void main()
{
    vertex_position_world_space = vec3(Model * vec4(v_position,1));
	vertex_normal_world_space = normalize(vec3(Model * vec4(v_normal,0)));


    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
