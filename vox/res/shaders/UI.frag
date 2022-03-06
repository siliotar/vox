#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int ElementID;

in vec2 v_TexCoord;
in flat int v_ElementID;

uniform sampler2D u_Texture;

void main()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//FragColor = texture(u_Texture, v_TexCoord);
	ElementID = int(v_ElementID);
}
