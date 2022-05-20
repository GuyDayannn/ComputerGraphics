#include "Renderer.h"
#include "MeshModel.h"
#include "ShaderProgram.h"
#include <imgui/imgui.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "PointLight.h"
#include <sstream>
#include "Utils.h"

Renderer::Renderer()
{
	cubeModel = Utils::LoadMeshModel("..\\Data\\cube.obj");
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



		if (scene->GetSkyBoxStatus())
		{
			//Drawing cubeMap
			glDepthFunc(GL_LEQUAL);
			cubeMapShader.use();
			cubeMapShader.setUniform("projection", camera.GetProjectionTransformation());
			cubeMapShader.setUniform("view", glm::mat4(glm::mat3(camera.GetViewTransformation())));

			cubeMapTexture.bind(0);
			glBindVertexArray(cubeModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, cubeModel->GetModelVertices().size());
			glDepthFunc(GL_LESS);
			cubeMapTexture.unbind(0);
		}



		//Drawing Models
		for(int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene->GetModel(currentModelIndex);
			if (scene->GetMirrorStatus() == false)
			{

				// Activate the 'colorShader' program (vertex and fragment shaders)
				colorShader.use();

				// Set the uniform variables
				colorShader.setUniform("model", currentModel->GetWorldTransformation() * currentModel->GetModelTransformation());
				colorShader.setUniform("view", camera.GetViewTransformation());
				colorShader.setUniform("projection", camera.GetProjectionTransformation());
				colorShader.setUniform("material.textureMap", 0);
				colorShader.setUniform("material.normalMap", 1);
				colorShader.setUniform("material.depthMap", 2);
				colorShader.setUniform("texType", currentModel->GetTextureMapKind());
				colorShader.setUniform("normalType", currentModel->GetNormalMapStatus());
				colorShader.setUniform("depthType", currentModel->GetDepthMapStatus());
				colorShader.setUniform("colType", currentModel->GetColorKind());
				colorShader.setUniform("camPos", camera.GetEye()); // active camera position
				colorShader.setUniform("lightCount", scene->GetLightCount()); // number of lights
				colorShader.setUniformSampler("toonShading", scene->GetToonShadingStatus());
				colorShader.setUniformSampler("toonLevels", scene->GetToonShadingLevels());

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
				depthTexture.bind(2);

				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);

				// Unset 'texture1' as the active texture at slot #0
				texture1.unbind(0);
				normalTextrue1.unbind(1);
				depthTexture.unbind(2);

				colorShader.setUniform("color", glm::vec3(0, 0, 0));

				/*
				// Drag our model's faces (triangles) in line mode (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
				*/
			}
			else
			{
				mirrorShader.use();
				mirrorShader.setUniform("model", currentModel->GetWorldTransformation()* currentModel->GetModelTransformation());
				mirrorShader.setUniform("view", camera.GetViewTransformation());
				mirrorShader.setUniform("projection", camera.GetProjectionTransformation());
				mirrorShader.setUniform("skybox", 0);
				mirrorShader.setUniform("camPos", camera.GetEye()); // active camera position

				cubeMapTexture.bind(0);
				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);

				// Unset 'texture1' as the active texture at slot #0
				cubeMapTexture.unbind(0);
			}
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
	cubeMapShader.loadShaders("vshader_cubemap.glsl", "fshader_cubemap.glsl");
	mirrorShader.loadShaders("vshader_mirror.glsl", "fshader_mirror.glsl");
}

void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("..\\Data\\bricks22.jpg", true))
	{
		texture1.loadTexture("..\\Data\\bricks22.jpg", true);
	}

	if (!normalTextrue1.loadTexture("..\\Data\\bricks22_normal.jpg", true))
	{
		normalTextrue1.loadTexture("..\\Data\\bricks22_normal.jpg", true);
	}

	if (!depthTexture.loadTexture("..\\Data\\bricks22_disp.jpg", true))
	{
		depthTexture.loadTexture("..\\Data\\bricks22_disp.jpg", true);
	}


	cubeMapTexture.loadCubeMap();

}