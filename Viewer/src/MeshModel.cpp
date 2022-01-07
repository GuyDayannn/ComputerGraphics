#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& modelName) :
	modelTransform(1),
	worldTransform(1),
	modelName(modelName)
{
	translation.push_back(glm::mat4x4(1.0f));
	translation.push_back(glm::mat4x4(1.0f));
	rotation.push_back(glm::mat4x4(1.0f));
	rotation.push_back(glm::mat4x4(1.0f));
	scale.push_back(glm::mat4x4(1.0f));
	scale.push_back(glm::mat4x4(1.0f));

	worldTransform = glm::mat4x4(1);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);

		glm::vec3 tangent = glm::vec3(1,1,1), bitangent = glm::vec3(1,1,1);
		if (textureCoords.size() > 0)//textures exist
		{

			//Calculating tangents and bitangents for normal map - learnt from learnopengl.com
			//positions
			int p1i = currentFace.GetVertexIndex(0) - 1;
			int p2i = currentFace.GetVertexIndex(1) - 1;
			int p3i = currentFace.GetVertexIndex(2) - 1;
			glm::vec3 p1 = vertices[p1i];
			glm::vec3 p2 = vertices[p2i];
			glm::vec3 p3 = vertices[p3i];
			//texture cordinates
			int p1ti = currentFace.GetTextureIndex(0) - 1;
			int p2ti = currentFace.GetTextureIndex(1) - 1;
			int p3ti = currentFace.GetTextureIndex(2) - 1;
			glm::vec2 uv1 = textureCoords[p1ti];
			glm::vec2 uv2 = textureCoords[p2ti];
			glm::vec2 uv3 = textureCoords[p3ti];
			//normal vector
			//triangle's edges and delta UV coordinates
			glm::vec3 edge1 = p2 - p1;
			glm::vec3 edge2 = p3 - p1;
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;
			//actual tangent and bitangent calculations
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		}



		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			
			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			vertex.normal = normals[vertexIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];

				vertex.tangent = tangent;
				vertex.bitangent = bitangent;
			}

			modelVertices.push_back(vertex);
		}
	}

	matriel.ambientColor = glm::vec3(1.0, 1.0f, 1.0f);
	matriel.diffuseColor = glm::vec3(1.0, 1.0f, 1.0f);
	matriel.specularColor = glm::vec3(1.0, 1.0f, 1.0f);
	matriel.shininess = 8.0f;

	textureMapKind = UV;
	colorKind = TEXTURE;
	normalMap = false;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// BiTangents
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);



	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const glm::vec3& MeshModel::GetColor() const
{
	return color;
}

void MeshModel::SetColor(const glm::vec3& color)
{
	this->color = color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}

const int MeshModel::GetTextureMapKind() const
{
	return textureMapKind;
}

void MeshModel::SetTextureMapKind(int kind) 
{
	textureMapKind = kind;
}

const int MeshModel::GetColorKind() const
{
	return colorKind;
}

void MeshModel::SetColorKind(int kind)
{
	colorKind = kind;
}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::SetModelTransformation(const glm::mat4x4& worldTransform)
{
	this->modelTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetModelTransformation() const
{
	return modelTransform;
}

void MeshModel::TranslateModel(const glm::vec3& translationVector)
{
	translation[MODEL] = glm::translate(translation[MODEL], translationVector);
	UpdateModelTransform();
	//modelTransform = glm::translate(glm::mat4(1.0f), translationVector) * modelTransform;
}

void MeshModel::TranslateWorld(const glm::vec3& translationVector)
{
	translation[WORLD] = glm::translate(translation[WORLD], translationVector);
	UpdateWorldTransform();
	//worldTransform = glm::translate(glm::mat4(1.0f), translationVector) * worldTransform;
}

void MeshModel::RotateXModel(float angle)
{
	rotation[MODEL] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)) * rotation[MODEL];
	UpdateModelTransform();
	//modelTransform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)) * modelTransform;
}

void MeshModel::RotateYModel(float angle)
{
	rotation[MODEL] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * rotation[MODEL];
	UpdateModelTransform();
	//modelTransform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * modelTransform;
}

void MeshModel::RotateZModel(float angle)
{
	rotation[MODEL] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * rotation[MODEL];
	UpdateModelTransform();
	//modelTransform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * modelTransform;
}

void MeshModel::ScaleXModel(float factor)
{
	scale[MODEL] = glm::scale(scale[MODEL], glm::vec3(factor, 1.0f, 1.0f));
	UpdateModelTransform();
	//modelTransform = glm::scale(modelTransform, glm::vec3(factor, 1.0f, 1.0f));
}

void MeshModel::ScaleYModel(float factor)
{
	scale[MODEL] = glm::scale(scale[MODEL], glm::vec3(1.0f, factor, 1.0f));
	UpdateModelTransform();
	//modelTransform = glm::scale(modelTransform, glm::vec3( 1.0f, factor, 1.0f));
}

void MeshModel::ScaleZModel(float factor)
{
	scale[MODEL] = glm::scale(scale[MODEL], glm::vec3(1.0f, 1.0f, factor));
	UpdateModelTransform();
	//modelTransform = glm::scale(modelTransform, glm::vec3(1.0f, 1.0f, factor));
}

void MeshModel::ScaleModel(float factor)
{
	scale[MODEL] = glm::scale(scale[MODEL], glm::vec3(factor, factor, factor));
	UpdateModelTransform();
	//modelTransform = glm::scale(modelTransform, glm::vec3(factor, factor, factor));
}

void MeshModel::RotateXWorld(float angle)
{
	rotation[WORLD] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)) * rotation[WORLD];
	UpdateWorldTransform();
	//worldTransform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)) * worldTransform;
}

void MeshModel::RotateYWorld(float angle)
{
	rotation[WORLD] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * rotation[WORLD];
	UpdateWorldTransform();
	//worldTransform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * worldTransform;
}

void MeshModel::RotateZWorld(float angle)
{
	rotation[WORLD] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * rotation[WORLD];
	UpdateWorldTransform();
	//worldTransform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * worldTransform;
}

void MeshModel::ScaleXWorld(float factor)
{
	scale[WORLD] = glm::scale(scale[WORLD], glm::vec3(factor, 1.0f, 1.0f));
	UpdateWorldTransform();
	//worldTransform = glm::scale(worldTransform, glm::vec3(factor, 1.0f, 1.0f));
}

void MeshModel::ScaleYWorld(float factor)
{
	scale[WORLD] = glm::scale(scale[WORLD], glm::vec3(1.0f, factor, 1.0f));
	UpdateWorldTransform();
	//worldTransform = glm::scale(worldTransform, glm::vec3(1.0f, factor, 1.0f));
}

void MeshModel::ScaleZWorld(float factor)
{
	scale[WORLD] = glm::scale(scale[WORLD], glm::vec3(1.0f, 1.0f, factor));
	UpdateWorldTransform();
	//worldTransform = glm::scale(worldTransform, glm::vec3(1.0f, 1.0f, factor));
}

void MeshModel::ScaleWorld(float factor)
{
	scale[WORLD] = glm::scale(scale[WORLD], glm::vec3(factor, factor, factor));
	UpdateWorldTransform();
	//worldTransform = glm::scale(worldTransform, glm::vec3(factor, factor, factor));
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}

Material& MeshModel::GetMaterial()
{
	return matriel;
}

void MeshModel::UpdateModelTransform()
{
	modelTransform = translation[MODEL] * rotation[MODEL] * scale[MODEL];
}

void MeshModel::UpdateWorldTransform()
{
	worldTransform = translation[WORLD] * rotation[WORLD] * scale[WORLD];
}

const bool MeshModel::GetNormalMapStatus() const
{
	return normalMap;
}

void MeshModel::SetNormalMapStatus(bool status)
{
	normalMap = status;
}