#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::vec3& GetPureVertex(int index) const;
	const glm::vec3& GetTransformedVertex(int index) const;
	static glm::vec4 Vec3ToHomogeneousVec(const glm::vec3& vec);
	static glm::vec3 HomogeneousVecToVec3(const glm::vec4& vec);
	void FitToWindow(int viewport_width, int viewport_height);
	void UpdateWorldTransformations(const glm::vec3& scale, const glm::vec3& rotate, const glm::vec3& translate);
	void UpdateModelTransformations(const glm::vec3& scale, const glm::vec3& rotate, const glm::vec3& translate);

private:
	std::vector<glm::vec3> scaling; //vec.size() = 2 [0] - world, [1] - model , scaling[i] = {xscale, yscale, zscale}
	std::vector<glm::vec3> rotation;
	std::vector<glm::vec3> translation;
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
};
