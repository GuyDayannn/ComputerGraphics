#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Light.h"
#include "MeshModel.h"

class PointLight : public Light, public MeshModel
{
public:
	PointLight(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName = "");
	PointLight(const MeshModel& model, const glm::vec3& color);
	virtual ~PointLight();
	glm::vec3& GetPosition();
	glm::vec3 GetCenter();

private:
	glm::vec3 position;
};
