#include "Light.h"

Light::Light() :
	color(glm::vec3(1.0, 1.0, 1.0))
{
	ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
	diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
	specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
	ambientIntensity = 0.5;
	diffuseIntensity = 0.5;
	specularIntensity = 0.5;
}

Light::Light(const glm::vec3& color) :
	color(color)
{
	ambientColor = glm::vec3(0.0f, 0.0f, 0.0f);
	diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
	specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
	ambientIntensity = 0.5;
	diffuseIntensity = 0.5;
	specularIntensity = 0.5;
}


Light::~Light()
{
}

const glm::vec3& Light::GetColor() const
{
	return color;
}

void Light::SetColor(const glm::vec3& color)
{
	this->color = color;
}

const glm::vec3& Light::GetAmbientColor()  const
{
	return ambientColor;
}

void Light::SetAmbientColor(const glm::vec3& color)
{
	ambientColor = color;
}

const glm::vec3& Light::GetDiffuseColor()  const
{
	return diffuseColor;
}

void Light::SetDiffuseColor(const glm::vec3& color)
{
	diffuseColor = color;
}

const glm::vec3& Light::GetSpecularColor()  const
{
	return specularColor;
}

void Light::SetSpecularColor(const glm::vec3& color)
{
	specularColor = color;
}

const float Light::GetAmbientIntensity()  const
{
	return ambientIntensity;
}

void Light::SetAmbientIntensity(float factor)
{
	ambientIntensity = factor;
}

const float Light::GetDiffuseIntensity()  const
{
	return diffuseIntensity;
}

void Light::SetDiffuseIntensity(float factor)
{
	diffuseIntensity = factor;
}

const float Light::GetSpecularIntensity()  const
{
	return specularIntensity;
}

void Light::SetSpecularIntensity(float factor)
{
	specularIntensity = factor;
}