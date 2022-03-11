#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in float v_Light;

uniform sampler2D u_Texture;

void main()
{
	vec4 tex = texture(u_Texture, v_TexCoord);
	vec3 color = vec3(v_Light * tex);
	FragColor = vec4(color, tex.w);
	//vec3 color = vec3(0.2f, 0.8f, 0.3f) * v_Light;
	//FragColor = vec4(color, 1.0f);
}
