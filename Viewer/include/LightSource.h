#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Face.h"
#include <iostream>

class LightSource
{
public:
	LightSource(int num);
	virtual ~LightSource();

	std::string GetName() const;

	//Utils
	static glm::vec4 Vec3ToHomogeneousVec(const glm::vec3& vec);
	static glm::vec3 HomogeneousVecToVec3(const glm::vec4& vec);
	void UpdateRotationWorld(float degrees, std::string axis);
	void UpdateRotationModel(float degrees, std::string axis);
	void UpdateTranslationWorld(const glm::vec3& vec);
	void UpdateTranslationModel(const glm::vec3& vec);
	void UpdateScalingWorld(const glm::vec3& vec);
	void UpdateScalingModel(const glm::vec3& vec);

	glm::vec3 GetPosition() const;
	void UpdatePosition(const glm::vec3& pos);

	void UpdateAmbientColor(const glm::vec3& color);
	const glm::vec3& GetAmbientColor() const;

	void UpdateDiffuseColor(const glm::vec3& color);
	const glm::vec3& GetDiffuseColor() const;

	void UpdateSpecularColor(const glm::vec3& color);
	const glm::vec3& GetSpecularColor() const;


private:
	std::string name;
	glm::vec3 position;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	std::vector<glm::mat4> currentTranslationMat;
	std::vector<glm::mat4> currentRotationMat; //[0] world - [1] - light itself
	std::vector<glm::mat4> currentScalingMat;

};