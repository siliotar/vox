#version 330 core

layout(location = 0) out vec4 FragColor;

flat in vec2 v_TexCoordStart;
in vec2 v_TexCoord;
in float v_Light;

uniform sampler2D u_Texture;

float textureSize = 32.0f / 512.0f;

void main()
{
	vec2 texCoords = vec2(v_TexCoordStart.x + textureSize * fract(v_TexCoord.x), v_TexCoordStart.y + textureSize * fract(v_TexCoord.y));
	vec4 tex = texture(u_Texture, texCoords);
	vec3 color = vec3(v_Light * tex);
	FragColor = vec4(color, tex.w);
}
