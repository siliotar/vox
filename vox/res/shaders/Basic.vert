#version 330 core

layout (location = 0) in uint vertexData;

out vec2 v_TexCoord;
out float v_Light;

uniform mat4 MVP;
uniform ivec2 chunkCoord;

vec2 texCoords[4] = vec2[4](
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
);

float textureSize = 32.0f / 512.0f;
uint atlasXSize = 16u;
uint atlasYSize = 16u;

void main()
{
	float posX = float(chunkCoord.x) + float(vertexData & 0x1fu);
	float posY = float((vertexData >> 5u) & 0x1ffu) - 1.5f;
	float posZ = float(chunkCoord.y) + float((vertexData >> 14u) & 0x1fu);
	
	vec2 texCoord = texCoords[(vertexData >> 19u) & 0x3u];
	uint texID = (vertexData >> 21u) & 0x1ffu;
	float textureX = float(texID % atlasXSize) * textureSize + texCoord.x * textureSize;
	float textureY = float(atlasYSize - 1u - texID / atlasYSize) * textureSize + texCoord.y * textureSize;

	gl_Position = MVP * vec4(posX, posY, posZ, 1.0f);
	v_TexCoord = vec2(textureX, textureY);
	v_Light = float((vertexData >> 30u) & 0x3u) * 0.2f + 0.4f;
}
