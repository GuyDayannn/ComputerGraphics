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
	worldTransform = glm::mat4x4(1);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
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
			}

			modelVertices.push_back(vertex);
		}
	}

	matriel.ambientColor = glm::vec3(0.0, 0.0f, 0.0f);
	matriel.diffuseColor = glm::vec3(0.0, 0.0f, 0.0f);
	matriel.specularColor = glm::vec3(0.0, 0.0f, 0.0f);

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
	modelTransform = glm::translate(glm::mat4(1.0f), translationVector) * modelTransform;
}

void MeshModel::TranslateWorld(const glm::vec3& translationVector)
{
	worldTransform = glm::translate(glm::mat4(1.0f), translationVector) * worldTransform;
}

void MeshModel::RotateXModel(float angle)
{
	modelTransform = glm::rotate(modelTransform, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void MeshModel::RotateYModel(float angle)
{
	modelTransform = glm::rotate(modelTransform, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void MeshModel::RotateZModel(float angle)
{
	modelTransform = glm::rotate(modelTransform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void MeshModel::ScaleXModel(float factor)
{
	modelTransform = glm::scale(modelTransform, glm::vec3(factor, 1.0f, 1.0f));
}

void MeshModel::ScaleYModel(float factor)
{
	modelTransform = glm::scale(modelTransform, glm::vec3( 1.0f, factor, 1.0f));
}

void MeshModel::ScaleZModel(float factor)
{
	modelTransform = glm::scale(modelTransform, glm::vec3(1.0f, 1.0f, factor));
}

void MeshModel::ScaleModel(float factor)
{
	modelTransform = glm::scale(modelTransform, glm::vec3(factor, factor, factor));
}

void MeshModel::RotateXWorld(float angle)
{
	worldTransform = glm::rotate(worldTransform, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void MeshModel::RotateYWorld(float angle)
{
	worldTransform = glm::rotate(worldTransform, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void MeshModel::RotateZWorld(float angle)
{
	worldTransform = glm::rotate(worldTransform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void MeshModel::ScaleXWorld(float factor)
{
	worldTransform = glm::scale(worldTransform, glm::vec3(factor, 1.0f, 1.0f));
}

void MeshModel::ScaleYWorld(float factor)
{
	worldTransform = glm::scale(worldTransform, glm::vec3(1.0f, factor, 1.0f));
}

void MeshModel::ScaleZWorld(float factor)
{
	worldTransform = glm::scale(worldTransform, glm::vec3(1.0f, 1.0f, factor));
}

void MeshModel::ScaleWorld(float factor)
{
	worldTransform = glm::scale(worldTransform, glm::vec3(factor, factor, factor));
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