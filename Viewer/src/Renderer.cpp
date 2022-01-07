#include "Renderer.h"
#include "MeshModel.h"
#include "ShaderProgram.h"
#include <imgui/imgui.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "PointLight.h"
#include <sstream>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	int cameraCount = scene->GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene->GetModelCount();
		const Camera& camera = scene->GetActiveCamera();
	
		for(int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene->GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

			// Set the uniform variables
			colorShader.setUniform("model", currentModel->GetWorldTransformation() * currentModel->GetModelTransformation());
			colorShader.setUniform("view", camera.GetViewTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			colorShader.setUniform("material.textureMap", 0);
			colorShader.setUniform("material.normalMap", 1);
			colorShader.setUniform("texType", currentModel->GetTextureMapKind());
			colorShader.setUniform("normalType", currentModel->GetNormalMapStatus());
			colorShader.setUniform("colType", currentModel->GetColorKind());
			colorShader.setUniform("camPos", camera.GetEye()); // active camera position
			colorShader.setUniform("lightCount", scene->GetLightCount()); // number of lights

			//Uniform variables for Matriel (model matriel)
			colorShader.setUniform("material.shininess", currentModel->GetMaterial().shininess);
			colorShader.setUniform("material.ambientColor", currentModel->GetMaterial().ambientColor);
			colorShader.setUniform("material.diffuseColor", currentModel->GetMaterial().diffuseColor);
			colorShader.setUniform("material.specularColor", currentModel->GetMaterial().specularColor);

			
			std::shared_ptr<PointLight> pLight;
			for (int i = 0; i < scene->GetLightCount(); i++)
			{
				std::shared_ptr<Light> lght = scene->GetLight(i);
				pLight = std::dynamic_pointer_cast<PointLight>(lght);

				if (pLight != NULL) //pointLight
				{
					std::ostringstream plts;
					plts << "lightTransfomation[" << i << "]";
					std::string mystring = plts.str();
					colorShader.setUniform(mystring.c_str(), pLight->GetWorldTransformation() * pLight->GetModelTransformation());
				}

				//creating and transfering everything needed for LightStruct (lightMatriel) and setting uniform variables
				std::ostringstream s;
				s << "lightMatriel[" << i << "]";
				std::string matString = s.str();

				std::string ambientC = matString + ".ambientColor";
				std::string diffuseC = matString + ".diffuseColor";
				std::string specularC = matString + ".specularColor";
				std::string ambientI = matString + ".diffuseIntensity";
				std::string diffuseI = matString + ".diffuseColor";
				std::string specularI = matString + ".specularIntensity";

				colorShader.setUniform(ambientC.c_str(), lght->GetAmbientColor());
				colorShader.setUniform(diffuseC.c_str(), lght->GetDiffuseColor());
				colorShader.setUniform(specularC.c_str(), lght->GetSpecularColor());
				colorShader.setUniform(ambientI.c_str(), lght->GetAmbientIntensity());
				colorShader.setUniform(diffuseI.c_str(), lght->GetDiffuseIntensity());
				colorShader.setUniform(specularI.c_str(), lght->GetSpecularIntensity());
			}

			// Set 'texture1' as the active texture at slot #0
			texture1.bind(0);
			normalTextrue1.bind(1);

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);

			// Unset 'texture1' as the active texture at slot #0
			texture1.unbind(0);
			normalTextrue1.unbind(1);

			colorShader.setUniform("color", glm::vec3(0,0,0));

			/*
			// Drag our model's faces (triangles) in line mode (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);
			*/
		}


		//Drawing Lights
		if (scene->GetLightsStatus())
		{
			int lightC = scene->GetLightCount();
			for (int i = 0; i < lightC; i++)
			{
				std::shared_ptr<Light> currentLight = scene->GetLight(i);
				std::shared_ptr<PointLight> pLight;
				pLight = std::dynamic_pointer_cast<PointLight>(currentLight);

				if (pLight != NULL)
				{
					// Activate the 'lightShader' program (vertex and fragment shaders)
					lightShader.use();

					// Set the uniform variables
					lightShader.setUniform("model", pLight->GetWorldTransformation() * pLight->GetModelTransformation());
					lightShader.setUniform("view", camera.GetViewTransformation());
					lightShader.setUniform("projection", camera.GetProjectionTransformation());
					lightShader.setUniform("material.textureMap", 0);
					lightShader.setUniform("camPos", camera.GetEye()); // active camera position

					// Set 'texture1' as the active texture at slot #0
					texture1.bind(0);

					// Drag our model's faces (triangles) in fill mode
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glBindVertexArray(pLight->GetVAO());
					glDrawArrays(GL_TRIANGLES, 0, pLight->GetModelVertices().size());
					glBindVertexArray(0);

					// Unset 'texture1' as the active texture at slot #0
					texture1.unbind(0);

				}

			}
		}
	}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
	lightShader.loadShaders("vshader_light.glsl", "fshader_light.glsl");
}

void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("..\\Data\\mars.png", true))
	{
		texture1.loadTexture("..\\Data\\mars.png", true);
	}

	if (!normalTextrue1.loadTexture("..\\Data\\normalwall.png", true))
	{
		normalTextrue1.loadTexture("..\\Data\\normalwall.png", true);
	}

}