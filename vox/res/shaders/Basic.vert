#version 330 core

layout (location = 0) in uvec2 vertexData;

flat out vec2 v_TexCoordStart;
out vec2 v_TexCoord;
out float v_Light;

uniform mat4 MVP;
uniform ivec3 chunkCoord;

vec3 positions[24] = vec3[24](
	// Up
	vec3(0.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),

	// Left
	vec3(1.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(1.0f, 1.0f, 1.0f),

	// Front
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(1.0f, 1.0f, 0.0f),

	// Right
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 1.0f, 1.0f),
	vec3(0.0f, 1.0f, 0.0f),

	// Back
	vec3(0.0f, 0.0f, 1.0f),
	vec3(1.0f, 0.0f, 1.0f),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(0.0f, 1.0f, 1.0f),

	// Down
	vec3(0.0f, 0.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(1.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f)
);

vec2 texCoords[4] = vec2[4](
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
);

float textureSize = 32.0f / 512.0f;
uint atlasXSize = 16u;
uint atlasYSize = 16u;

vec2 getTextureScale(uint normal, uint scaleX, uint scaleY, uint scaleZ)
{
	float x, y;
	if (normal == 0 || normal == 2 || normal == 4 || normal == 5)
		x = scaleX;
	else
		x = scaleZ;
	if (normal > 0 && normal < 5)
		y = scaleY;
	else
		y = scaleZ;
	return vec2(float(x), float(y));
}

/*  111    111111111  |   11      1111      1111      1111    11111   11111   11111 */
/* Normal    texID    |  light   scaleZ    scaleY    scaleX   posZ    posY    posX  */

void main()
{
	uint posX = vertexData.x & 0x1fu;
	uint posY = (vertexData.x >> 5u) & 0x1fu;
	uint posZ = (vertexData.x >> 10u) & 0x1fu;
	uint scaleX = (vertexData.x >> 15u) & 0x1fu;
	uint scaleY = (vertexData.x >> 20u) & 0x1fu;
	uint scaleZ = (vertexData.x >> 25u) & 0x1fu;
	uint light = (vertexData.x >> 30u) & 0x3u;
	uint texID = vertexData.y & 0x1ffu;
	uint normal = (vertexData.y >> 9u) & 0x7u;

	vec3 position = positions[normal * 4u + gl_VertexID % 4];
	float worldPosX = float(chunkCoord.x) + float(posX) + float(scaleX) * position.x;
	float worldPosY = float(chunkCoord.y) + float(posY) + float(scaleY) * position.y - 1.5f;
	float worldPosZ = float(chunkCoord.z) + float(posZ) + float(scaleZ) * position.z;

	vec2 texCoord = texCoords[gl_VertexID % 4];
	float textureX = float(texID % atlasXSize) * textureSize;
	float textureY = float(atlasYSize - 1u - texID / atlasYSize) * textureSize;

	v_TexCoordStart = vec2(textureX, textureY);
	vec2 texScale = getTextureScale(normal, scaleX, scaleY, scaleZ);
	v_TexCoord = vec2(texCoord.x * texScale.x, texCoord.y * texScale.y);

	gl_Position = MVP * vec4(worldPosX, worldPosY, worldPosZ, 1.0f);
	v_Light = float(light) * 0.2f + 0.4f;
}
