#include "LightSource.h"

LightSource::LightSource(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, int num):
	MeshModel(faces, vertices, normals, model_name)
{
	activeLightType = 0;
	UpdateModelTransformations(glm::vec3(0.1, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), "x", glm::vec3(0.0f, 0.0f, 0.0f));
	ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ActivateAmbient();
	position = GetCenter();
	shadingType = 0;
	ambientIntensity = 0.5f;
	diffusiveIntensity = 0.5f;
	specularIntensity = 0.5f;
	name = "Light " + std::to_string(num);
}

LightSource::~LightSource()
{
}

const std::string& LightSource::GetName() const
{
	return name;
}

void LightSource::UpdateName(std::string n) 
{
	name = n;
}

glm::vec3 LightSource::GetPosition() const
{
	return position;
}

void LightSource::UpdatePosition(const glm::vec3& pos)
{
	position = pos;
}

void LightSource::UpdateAmbientColor(const glm::vec3& color)
{
	ambientColor = color;
	if (activeLightType == 0) activeColor = color;
}

const glm::vec3& LightSource:: GetAmbientColor() const
{
	return ambientColor;
}

void LightSource::UpdateDiffuseColor(const glm::vec3& color)
{
	diffuseColor = color;
	if (activeLightType == 1) activeColor = color;
}

const glm::vec3& LightSource::GetDiffuseColor() const
{
	return diffuseColor;
}

void LightSource::UpdateSpecularColor(const glm::vec3& color)
{
	specularColor = color;
	if (activeLightType == 2) activeColor = color;
}

const glm::vec3& LightSource::GetSpecularColor() const
{
	return specularColor;
}

void LightSource::ActivateAmbient()
{
	activeLightType = 0;
	activeColor = ambientColor;
}

void LightSource::ActivateDiffuse()
{
	activeLightType = 1;
	activeColor = diffuseColor;
}

void LightSource::ActivateSpecular()
{
	activeLightType = 2;
	activeColor = specularColor;
}

const glm::vec3 LightSource::GetActiveColor() const
{
	return activeColor;
}

glm::vec3 LightSource::GetCenter() const
{
	float minX = vertices[0].x, minY = vertices[0].y, minZ = vertices[0].z;
	float maxX = minX, maxY = minY, maxZ = minZ;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x > maxX)
			maxX = vertices[i].x;

		if (vertices[i].x < minX)
			minX = vertices[i].x;

		if (vertices[i].y > maxY)
			maxY = vertices[i].y;

		if (vertices[i].y < minY)
			minY = vertices[i].y;

		if (vertices[i].z > maxZ)
			maxZ = vertices[i].z;

		if (vertices[i].z < minZ)
			minZ = vertices[i].z;

	}
	return glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
}

const glm::vec3 LightSource::GetTransformedPosition() const
{
	std::vector<glm::mat4> scalingMats = GetScalingMatrices();
	std::vector<glm::mat4> rotationMats = GetRotationMatrices();
	std::vector<glm::mat4> translationMats = GetTranslationMatrices();
	glm::mat4 worldTransformationMat = translationMats[0] * currentRotationMat[0] * scalingMats[0];
	glm::mat4 modelTransformationMat = translationMats[1] * currentRotationMat[1] * scalingMats[1];

	return HomogeneousVecToVec3(worldTransformationMat * modelTransformationMat * Vec3ToHomogeneousVec(position));

}

void LightSource::UpdateShadingType(int type)
{
	shadingType = type;
}

const bool LightSource::IsFlat() const
{
	return shadingType == 0;
}

const bool LightSource::IsGouraud() const
{
	return shadingType == 1;
}

const bool LightSource::IsPhong() const
{
	return shadingType == 2;
}

void LightSource::UpdateAmbientIntensity(float val)
{
	ambientIntensity = val;
}

void LightSource::UpdateDiffusiveIntensity(float val)
{
	diffusiveIntensity = val;
}

void LightSource::UpdateSpecularIntensity(float val)
{
	specularIntensity = val;
}

float LightSource::GetAmbientIntensity() const
{
	return ambientIntensity;
}

float LightSource::GetDiffusiveIntensity() const
{
	return diffusiveIntensity;
}

float LightSource::GetSpecularIntensity() const
{
	return specularIntensity;
}