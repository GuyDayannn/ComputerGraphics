#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"

enum tex {UV, PLANE, CYLINDER, SPHERE};
enum col {COLOR, TEXTURE};

struct Material
{
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
protected:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> textureCoords;

	std::vector<Vertex> modelVertices;

	glm::mat4x4 modelTransform;
	glm::mat4x4 worldTransform;

	std::string modelName;

	glm::vec3 color;
	Material matriel;

	GLuint vbo;
	GLuint vao;

	int textureMapKind; // 0 - CORD 1 - PLANE 2 - CYLINDER 3 - SPHERE
	int colorKind; // 0 - color - 1 - texture

public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName = "");
	virtual ~MeshModel();

	const glm::mat4x4& GetWorldTransformation() const;
	const glm::mat4x4& GetModelTransformation() const;

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	void SetModelTransformation(const glm::mat4x4& modelTransform);

	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& color);

	const std::string& GetModelName();

	const int GetTextureMapKind() const;
	void SetTextureMapKind(int kind);


	const int GetColorKind() const;
	void SetColorKind(int kind);

	const std::vector<Vertex>& GetModelVertices();

	void TranslateModel(const glm::vec3& translationVector);
	void TranslateWorld(const glm::vec3& translationVector);

	void RotateXModel(float angle);
	void RotateYModel(float angle);
	void RotateZModel(float angle);
	void ScaleXModel(float factor);
	void ScaleYModel(float factor);
	void ScaleZModel(float factor);
	void ScaleModel(float factor);

	void RotateXWorld(float angle);
	void RotateYWorld(float angle);
	void RotateZWorld(float angle);
	void ScaleXWorld(float factor);
	void ScaleYWorld(float factor);
	void ScaleZWorld(float factor);
	void ScaleWorld(float factor);

	GLuint GetVAO() const;

	Material& GetMaterial();
};
