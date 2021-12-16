#include "LightSource.h"

LightSource::LightSource(int num)
{
	ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	currentRotationMat.push_back(glm::mat4(1.0f));
	currentRotationMat.push_back(glm::mat4(1.0f));
	currentTranslationMat.push_back(glm::mat4(1.0f));
	currentTranslationMat.push_back(glm::mat4(1.0f));
	currentScalingMat.push_back(glm::mat4(1.0f));
	currentScalingMat.push_back(glm::mat4(1.0f));
	name = "Light " + std::to_string(num);
}

LightSource::~LightSource()
{
}

std::string LightSource::GetName() const
{
	return name;
}


glm::vec4 LightSource::Vec3ToHomogeneousVec(const glm::vec3& vec)
{
	return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}

//converting Homogeneous Vector to vec3
glm::vec3 LightSource::HomogeneousVecToVec3(const glm::vec4& vec)
{
	if (vec.w == 0)
		return glm::vec3(vec.x, vec.y, vec.z);
	return glm::vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

void LightSource::UpdateRotationWorld(float degrees, std::string axis)
{
	glm::vec3 rotateAround;
	if (axis == "z") rotateAround = glm::vec3(0.0f, 0.0f, 1.0f);
	else if (axis == "y") rotateAround = glm::vec3(0.0f, 1.0f, 0.0f);
	else rotateAround = glm::vec3(1.0f, 0.0f, 0.0f);

	currentRotationMat[0] = glm::rotate(currentRotationMat[0], glm::radians(degrees), rotateAround);
}

void LightSource::UpdateRotationModel(float degrees, std::string axis)
{
	glm::vec3 rotateAround;
	if (axis == "z") rotateAround = glm::vec3(0.0f, 0.0f, 1.0f);
	else if (axis == "y") rotateAround = glm::vec3(0.0f, 1.0f, 0.0f);
	else rotateAround = glm::vec3(1.0f, 0.0f, 0.0f);

	currentRotationMat[1] = glm::rotate(currentRotationMat[1], glm::radians(degrees), rotateAround);
}

void LightSource::UpdateTranslationWorld(const glm::vec3& vec)
{
	currentTranslationMat[0] = glm::translate(glm::mat4(1.0f), vec);
}

void LightSource::UpdateTranslationModel(const glm::vec3& vec)
{
	currentTranslationMat[1] = glm::translate(glm::mat4(1.0f), vec);
}

void LightSource::UpdateScalingWorld(const glm::vec3& vec)
{
	currentScalingMat[0] = glm::scale(glm::mat4(1.0f), vec);
}

void LightSource::UpdateScalingModel(const glm::vec3& vec)
{
	currentScalingMat[1] = glm::scale(glm::mat4(1.0f), vec);
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
}

const glm::vec3& LightSource:: GetAmbientColor() const
{
	return ambientColor;
}

void LightSource::UpdateDiffuseColor(const glm::vec3& color)
{
	diffuseColor = color;
}

const glm::vec3& LightSource::GetDiffuseColor() const
{
	return diffuseColor;
}

void LightSource::UpdateSpecularColor(const glm::vec3& color)
{
	specularColor = color;
}

const glm::vec3& LightSource::GetSpecularColor() const
{
	return specularColor;
}