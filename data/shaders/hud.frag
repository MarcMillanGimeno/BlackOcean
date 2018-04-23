#version 330
in vec2 v_uv;
uniform sampler2D u_texture_diffuse;

out vec4 fragColor;

void main(void)
{
	fragColor =  vec4(texture2D(u_texture_diffuse,v_uv));
}
