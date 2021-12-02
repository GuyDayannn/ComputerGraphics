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
	int GetVerticesCount() const;
	const std::string& GetModelName() const;
	const std::vector<std::vector<std::vector<glm::vec3>>> GetVerticesNormals() const;
	const std::vector<std::vector<glm::vec3>> GetFacesNormals() const;
	const glm::vec3& GetPureVertex(int index) const;
	const glm::vec3& GetTransformedVertex(int index) const;
	const glm::vec3& GetPureNormal(int index) const;
	const glm::vec3& GetTransformedNormal(int index) const;
	static glm::vec4 Vec3ToHomogeneousVec(const glm::vec3& vec);
	static glm::vec3 HomogeneousVecToVec3(const glm::vec4& vec);
	//const std::vector<glm::vec3> FitToWindow(int viewport_width, int viewport_height) const; //[0] - scale fit, [1] - translate fit
	void UpdateWorldTransformations(const glm::vec3& scale, const glm::vec3& rotate, std::string axis, const glm::vec3& translate);
	void UpdateModelTransformations(const glm::vec3& scale, const glm::vec3& rotate, std::string axis, const glm::vec3& translate);
	const std::vector<glm::mat4> GetScalingMatrices() const;
	std::vector<glm::mat4> GetScalingMatricesChangeable() const;
	const std::vector<glm::mat4> GetTranslationMatrices() const;
	//rotation around "axis" to grid system "grid" (geid system means model grid or world grid)
	const std::vector<glm::mat4> GetRotationMatrices(const std::string axis = "z", const int grid = 1) const;
	const std::vector<std::vector<glm::vec3>> GetTriangles() const;
	const std::vector<glm::vec3>& GetFaceColors() const;
	const std::vector<std::vector<glm::vec3>> GetModelAxis() const;
	const std::vector<std::vector<glm::vec3>> GetTransformedModelAxisWorld() const;
	const std::vector<std::vector<glm::vec3>> GetTransformedModelAxisModel() const;
	void ShowWorldAxis();
	void HideWorldAxis();
	const bool GetWorldAxisShowState() const;
	void ShowModelAxis();
	void HideModelAxis();
	const bool GetModelAxisShowState() const;
	const std::vector<glm::mat4> GetCurrentRotation() const;
	void SetVertexNormalSize(float size);
	void SetFaceNormalSize(float size);
	bool GetFaceNormalsShowState();
	void ShowFaceNormals();
	void HideFaceNormals();
	bool GetVertexNormalsShowState();
	void ShowVertexNormals();
	void HideVertexNormals();
	void UpdateAxisScale(bool perspectiveProj);

	bool displayBoundingBox;
	bool displayBoundingRec;

private:
	std::vector<glm::vec3> translation; //vec.size() = 2 [0] - world, [1] - model , scaling[i] = {xscale, yscale, zscale}
	std::vector<glm::vec3> rotation; // degrees
	std::vector<glm::vec3> scaling;
	std::vector<glm::mat4> currentRotationMat;
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
	std::vector<std::vector<glm::vec3>> modelAxis;
	std::vector<std::vector<glm::vec3>> modelAxisModel;
	std::vector<glm::vec3> colors; //color for each face / triangle
	bool showAxisWorld;
	bool showAxisModel;
	float faceNormalSize;
	float vertexNormalSize;
	bool showFaceNormals;
	bool showVertexNormals;
};
