#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform float CurrentTime;
uniform int light_mode;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    if (light_mode == 0 || light_mode == 3) {
        vec4 color1 = texture2D(texture_1, texcoord + vec2(CurrentTime * 0.1, 0));
        out_color = color1;
    } else {
        out_color = vec4(0);    
    }

}