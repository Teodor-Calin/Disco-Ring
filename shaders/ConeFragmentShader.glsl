#version 330

// Input
in vec3 color1;

uniform int light_mode;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    if (light_mode == 0 || light_mode == 2) {
        out_color = vec4(color1, 0.5);
    } else {
        out_color = vec4(0);
    }

    
}
