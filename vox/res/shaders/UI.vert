#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in int ID;

out vec2 v_TexCoord;
flat out int v_ElementID;

void main()
{
	gl_Position = vec4(position.x, position.y, 1.0f, 1.0f);
	v_TexCoord = texCoord;
	v_ElementID = int(ID);
}
