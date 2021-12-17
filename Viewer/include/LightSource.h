#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Face.h"
#include "MeshModel.h"
#include <iostream>

class LightSource: public MeshModel
{
public:
	LightSource(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, int num = 0);
	virtual ~LightSource();

	const std::string& GetName() const;
	void UpdateName(std::string n);

	glm::vec3 GetPosition() const;
	void UpdatePosition(const glm::vec3& pos);

	void UpdateAmbientColor(const glm::vec3& color);
	const glm::vec3& GetAmbientColor() const;

	void UpdateDiffuseColor(const glm::vec3& color);
	const glm::vec3& GetDiffuseColor() const;

	void UpdateSpecularColor(const glm::vec3& color);
	const glm::vec3& GetSpecularColor() const;

	void ActivateAmbient();
	void ActivateDiffuse();
	void ActivateSpecular();

	const glm::vec3 GetActiveColor() const;
	glm::vec3 GetCenter() const;
	const glm::vec3 GetTransformedPosition() const;


	int activeLightType; // 0 - ambient , 1 - diffuse, 2 - specular
private:
	std::string name;
	glm::vec3 position;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 activeColor;

};