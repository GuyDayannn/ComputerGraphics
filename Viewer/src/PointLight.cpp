#include "PointLight.h"
#include <algorithm>

PointLight::PointLight(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName) :
	MeshModel(faces, vertices, normals, textureCoords, modelName),
	Light()
{
	ScaleModel(0.1f);
	position = GetCenter();
}

PointLight::PointLight(const MeshModel& model, const glm::vec3& color) :
	MeshModel(model),
	Light(color)
{
	ScaleModel(0.1f);
	position = GetCenter();
}

PointLight::~PointLight()
{
}

glm::vec3& PointLight::GetPosition()
{
	return position;
}

glm::vec3 PointLight::GetCenter()
{
	float minX = modelVertices[0].position.x, minY = modelVertices[0].position.y, minZ = modelVertices[0].position.z;
	float maxX = minX, maxY = minY, maxZ = minZ;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (modelVertices[i].position.x > maxX)
			maxX = modelVertices[i].position.x;

		if (modelVertices[i].position.x < minX)
			minX = modelVertices[i].position.x;

		if (modelVertices[i].position.y > maxY)
			maxY = modelVertices[i].position.y;

		if (modelVertices[i].position.y < minY)
			minY = modelVertices[i].position.y;

		if (modelVertices[i].position.z > maxZ)
			maxZ = modelVertices[i].position.z;

		if (vertices[i].z < minZ)
			minZ = modelVertices[i].position.z;

	}

	return glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
}