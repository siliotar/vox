#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ElementID;

in vec2 v_TexCoord;
in flat int v_ElementID;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, v_TexCoord);
	ElementID = vec4(v_ElementID);
}
