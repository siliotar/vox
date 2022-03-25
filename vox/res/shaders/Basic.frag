#version 330 core

layout(location = 0) out vec4 FragColor;

flat in vec2 v_TexCoordStart;
in vec2 v_TexCoord;
in float v_Light;

uniform sampler2D u_Texture;

float textureSize = 32.0f / 512.0f;

void main()
{
	vec2 ddx = dFdx(v_TexCoordStart + textureSize * v_TexCoord);
	vec2 ddy = dFdy(v_TexCoordStart + textureSize * v_TexCoord);
	vec2 texCoords = v_TexCoordStart + textureSize * fract(v_TexCoord);
	vec4 tex = textureGrad(u_Texture, texCoords, ddx, ddy);
	vec3 color = vec3(v_Light * tex);
	FragColor = vec4(color, tex.w);
}
