#version 330 core

layout (location = 0) in uvec2 vertexData;

flat out vec2 v_TexCoordStart;
out vec2 v_TexCoord;
out float v_Light;

uniform mat4 MVP;
uniform ivec3 chunkCoord;

const vec3 positions[24] = vec3[24](
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

const float textureSize = 32.0f / 512.0f;
const uint atlasXSize = 16u;
const uint atlasYSize = 16u;

vec2 getTextureScale(uint normal, uint scaleX, uint scaleY, uint scaleZ)
{
	float x, y;
	if (normal == 0u || normal == 2u || normal == 4u || normal == 5u)
		x = scaleX;
	else
		x = scaleZ;
	if (normal > 0u && normal < 5u)
		y = scaleY;
	else
		y = scaleZ;
	return vec2(float(x), float(y));
}

/*   1111      1111      1111    111111111  |   111      11      1111      1111      1111    11111   11111   11111 */
/* clusterZ  clusterY  clusterX    texID    |  normal   light   scaleZ    scaleY    scaleX   posZ    posY    posX  */

void main()
{
	uint posX = vertexData.x & 0x1fu;
	uint posY = (vertexData.x >> 5u) & 0x1fu;
	uint posZ = (vertexData.x >> 10u) & 0x1fu;
	uint scaleX = ((vertexData.x >> 15u) & 0xfu) + 1u;
	uint scaleY = ((vertexData.x >> 19u) & 0xfu) + 1u;
	uint scaleZ = ((vertexData.x >> 23u) & 0xfu) + 1u;
	uint light = (vertexData.x >> 27u) & 0x3u;
	uint normal = (vertexData.x >> 29u) & 0x7u;
	uint texID = vertexData.y & 0x1ffu;

	int vertexIDMod4 = gl_VertexID % 4;

	vec3 position = positions[int(normal * 4u) + vertexIDMod4];
	float worldPosX = float(chunkCoord.x) + float(posX) + float(scaleX) * position.x;
	float worldPosY = float(chunkCoord.y) + float(posY) + float(scaleY) * position.y - 1.5f;
	float worldPosZ = float(chunkCoord.z) + float(posZ) + float(scaleZ) * position.z;

	float texX = 0.0f;
	float texY = 0.0f;
	if (vertexIDMod4 == 1 || vertexIDMod4 == 2)
		texX = 1.0f;
	if (vertexIDMod4 > 1)
		texY = 1.0f;

	vec2 texCoord = vec2(texX, texY);
	float textureX = float(texID % atlasXSize) * textureSize;
	float textureY = float(atlasYSize - 1u - texID / atlasYSize) * textureSize;

	v_TexCoordStart = vec2(textureX, textureY);
	vec2 texScale = getTextureScale(normal, scaleX, scaleY, scaleZ);
	v_TexCoord = vec2(texCoord.x * texScale.x, texCoord.y * texScale.y);

	gl_Position = MVP * vec4(worldPosX, worldPosY, worldPosZ, 1.0f);
	v_Light = float(light) * 0.2f + 0.4f;
}
