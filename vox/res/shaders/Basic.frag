#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in float v_Light;

uniform sampler2D u_Texture;

void main()
{
	vec3 color = vec3(v_Light * texture(u_Texture, v_TexCoord));
	FragColor = vec4(color, 1.0f);
}
