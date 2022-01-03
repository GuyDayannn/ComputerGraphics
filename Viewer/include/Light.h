#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(const glm::vec3& color);
	virtual ~Light();

	const glm::vec3& GetColor()  const;
	void SetColor(const glm::vec3& color);

	const glm::vec3& GetAmbientColor()  const;
	void SetAmbientColor(const glm::vec3& color);

	const glm::vec3& GetDiffuseColor()  const;
	void SetDiffuseColor(const glm::vec3& color);

	const glm::vec3& GetSpecularColor()  const;
	void SetSpecularColor(const glm::vec3& color);

	const float GetAmbientIntensity()  const;
	void SetAmbientIntensity(float factor);

	const float GetDiffuseIntensity()  const;
	void SetDiffuseIntensity(float factor);

	const float GetSpecularIntensity()  const;
	void SetSpecularIntensity(float factor);

protected:
	glm::vec3 color;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
};
