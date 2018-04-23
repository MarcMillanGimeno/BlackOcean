#version 330
in vec2 v_uv;

out vec4 fragColor;

uniform vec3 color;

void main(void)
{
	fragColor =  vec4(color, 1.0);
}
