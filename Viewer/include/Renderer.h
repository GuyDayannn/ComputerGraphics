#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>
#include "Texture2D.h"

class Renderer
{
private:
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	ShaderProgram cubeMapShader;
	ShaderProgram mirrorShader;
	Texture2D texture1;
	Texture2D normalTextrue1;
	Texture2D depthTexture;
	Texture2D cubeMapTexture;
	std::shared_ptr<MeshModel> cubeModel;

public:
	Renderer();
	~Renderer();

	void Render(const std::shared_ptr<Scene>& scene);
	void LoadShaders();
	void LoadTextures();
};
