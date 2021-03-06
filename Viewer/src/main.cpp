#define _USE_MATH_DEFINES
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <nfd.h>

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "ImguiMenus.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "Utils.h"

/*
** Model Fields for ImGui
*/
enum Axises { X, Y, Z };
static float modelAdditions = 0.0f;
static int axis = 0;
static bool xAxis = true;
static bool yAxis = false;
static bool zAxis = false;
static float translation = 0.0f;
static float rotation = 0.0f;
static float scale = 1.0f;
static bool startScene = false;

/*
** Camera Fields for ImGui
*/
static float camPos[3] = { 0,0,10 };

/*
** Light Fields for ImGui
*/
static float lightTransformation[3] = {0.0,0.0,0.0};
static int lightAxis = 0;
static float lightTranslation = 0.0f;
std::vector<std::shared_ptr<MeshModel>> lightMesh;

double zoomFactor = 1;
int windowWidth = 1280;
int windowHeight = 720;
char* windowTitle = "Mesh Viewer";
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool zoomChanged = false;
std::shared_ptr<Scene> scene;

ImGuiIO* imgui;
GLFWwindow* window;

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup();

static void GlfwErrorCallback(int error, const char* description);
void RenderFrame(GLFWwindow* window, std::shared_ptr<Scene> scene, Renderer& renderer, ImGuiIO& io);

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

float GetAspectRatio();
void DrawImguiMenus();
void HandleImguiInput();

int main(int argc, char **argv)
{

	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	scene = std::make_shared<Scene>();
	glm::vec3 eye = glm::vec3(0, 0, 10);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	Camera camera = Camera(eye, at, up, GetAspectRatio());
	scene->AddCamera(camera);

	std::shared_ptr<MeshModel> firstLightM = Utils::LoadMeshModel("..\\Data\\crate.obj");
	scene->AddLight(std::make_shared<PointLight>(*firstLightM, glm::vec3(0, 0, 0)));

	Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTextures();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus();
		ImGui::Render();
		HandleImguiInput();

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard)
	{
		if (imgui->KeysDown[49]) // 1
		{
			scene->GetActiveModel()->RotateXModel(M_PI / 200);
		}

		if (imgui->KeysDown[50]) // 2
		{
			scene->GetActiveModel()->RotateXModel(-M_PI / 200);
		}

		if (imgui->KeysDown[51]) // 3
		{
			scene->GetActiveModel()->RotateYModel(M_PI / 200);
		}

		if (imgui->KeysDown[52]) // 4
		{
			scene->GetActiveModel()->RotateYModel(-M_PI / 200);
		}

		if (imgui->KeysDown[53]) // 5
		{
			scene->GetActiveModel()->RotateZModel(M_PI / 200);
		}

		if (imgui->KeysDown[54]) // 6
		{
			scene->GetActiveModel()->RotateZModel(-M_PI / 200);
		}

		if (imgui->KeysDown[45]) // -
		{
			scene->GetActiveModel()->ScaleModel(1 / 1.01);
		}

		if (imgui->KeysDown[61]) // +
		{
			scene->GetActiveModel()->ScaleModel(1.01);
		}

		if (imgui->KeysDown[65]) // a
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(-0.02, 0, 0));
		}

		if (imgui->KeysDown[68]) // d
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0.02, 0, 0));
		}

		if (imgui->KeysDown[83]) // s
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, 0.02));
		}

		if (imgui->KeysDown[87]) // w
		{
			scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, -0.02));
		}
	}

	if (!imgui->WantCaptureMouse)
	{
		if (zoomChanged)
		{
			scene->GetActiveCamera().Zoom(zoomFactor);
			zoomChanged = false;
		}
	}
}

void Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
	scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

float GetAspectRatio()
{
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void DrawImguiMenus()
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	{
		ImGui::Begin("Scene Menu");
		if (ImGui::ColorEdit3("Clear Color", (float*)&clearColor))
		{
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}

		bool skybox = scene->GetSkyBoxStatus();
		if (ImGui::Checkbox("Skybox", &skybox))
		{
			scene->SetSkyBoxStatus(skybox);
		}
		ImGui::SameLine();
		bool mirror = scene->GetMirrorStatus();
		if (ImGui::Checkbox("Mirror World", &mirror))
		{
			scene->SetMirrorStatus(mirror);
		}


		bool toonShading = scene->GetToonShadingStatus();
		if (ImGui::Checkbox("Toon Shading", &toonShading))
		{
			scene->SetToonShadingStatus(toonShading);
		}
		
		if (toonShading)
		{
			ImGui::SameLine();
			int toonLevels = scene->GetToonShadingLevels();
			if (ImGui::SliderInt("Toon Levels", &toonLevels, 3, 10))
			{
				scene->SetToonShadingLevels(toonLevels);
			}
		}


		if (ImGui::CollapsingHeader("Cameras"))
		{
			if (ImGui::Button("Add Camera"))
			{

				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 2 * M_PI);
				std::uniform_real_distribution<double> dist2(2, 10);
				double angle = dist(mt);
				double radius = dist2(mt);

				glm::vec4 eye = glm::vec4(radius * glm::cos(angle), 0, radius * glm::sin(angle), 1);
				glm::vec4 at = glm::vec4(0, 0, 0, 1);
				glm::vec4 up = glm::vec4(0, 1, 0, 1);
				scene->AddCamera(Camera(-scene->GetActiveCamera().GetEye(), at, up, GetAspectRatio()));
			}

			const char** items;
			std::vector<std::string> cameraStrings;
			items = new const char*[scene->GetCameraCount()];
			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				std::ostringstream s;
				s << "Camera " << i;
				std::string mystring = s.str();
				cameraStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				items[i] = cameraStrings[i].c_str();
			}

			int currentCamera = scene->GetActiveCameraIndex();
			ImGui::Combo("Active Camera", &currentCamera, items, scene->GetCameraCount());

			if (currentCamera != scene->GetActiveCameraIndex())
			{
				scene->SetActiveCameraIndex(currentCamera);
				scene->GetActiveCamera().SetAspectRatio(GetAspectRatio());
				camPos[0] = scene->GetActiveCamera().GetEye()[0];
				camPos[1] = scene->GetActiveCamera().GetEye()[1];
				camPos[2] = scene->GetActiveCamera().GetEye()[2];
			}

			delete items;

			if (ImGui::SliderFloat3("Eye", camPos, -10.0f, 10.0f))
			{
				scene->GetActiveCamera().SetEye(glm::vec3(camPos[0], camPos[1], camPos[2]));
			}


			int newProjectionType = scene->GetActiveCamera().IsPrespective() ? 0 : 1;
			ImGui::RadioButton("Prespective", &newProjectionType, 0);
			ImGui::RadioButton("Orthographic", &newProjectionType, 1);

			if (newProjectionType == 0)
			{
				static float fovy = 1.0f;
				static float zNear = 0.1f;
				static float zFar= 100.0f;

				scene->GetActiveCamera().SwitchToPrespective();

				if (ImGui::SliderFloat("Fovy", &fovy, 0.0f, M_PI))
				{
					scene->GetActiveCamera().SetFovy(fovy);
				}

				if (ImGui::SliderFloat("Near", &zNear, 0.1f, 10.0f))
				{
					scene->GetActiveCamera().SetNear(zNear);
				}

				if (ImGui::SliderFloat("Far", &zFar, 1.0f, 100.0f))
				{
					scene->GetActiveCamera().SetFar(zFar);
				}
			}
			else if (newProjectionType == 1)
			{
				static float height = 5.0f;
				static float zNear = 0.1f;
				float zFar = 100.0f;

				scene->GetActiveCamera().SwitchToOrthographic();

				if (ImGui::SliderFloat("Height", &height, 0.0f, M_PI))
				{
					scene->GetActiveCamera().SetHeight(height);
				}

				if (ImGui::SliderFloat("Near", &zNear, 1.0f, 10.0f))
				{
					scene->GetActiveCamera().SetNear(zNear);
				}

				if (ImGui::SliderFloat("Far", &zFar, 1.0f, 10.0f))
				{
					scene->GetActiveCamera().SetFar(zFar);
				}
			}
		}

		if (ImGui::CollapsingHeader("Models"))
		{
			const char** items;
			std::vector<std::string> modelStrings;
			items = new const char*[scene->GetModelCount()];
			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				std::ostringstream s;
				s << scene->GetModel(i)->GetModelName();
				std::string mystring = s.str();
				modelStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				items[i] = modelStrings[i].c_str();
			}

			int currentModelIndex = scene->GetActiveModelIndex();
			ImGui::Combo("Active Model", &currentModelIndex, items, scene->GetModelCount());

			if (currentModelIndex != scene->GetActiveModelIndex())
			{
				scene->SetActiveModelIndex(currentModelIndex);
			}

			delete items;

			int texType = scene->GetActiveModel()->GetTextureMapKind();
			if (ImGui::RadioButton("UV Map", &texType, UV))
			{
				scene->GetActiveModel()->SetTextureMapKind(UV);
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Planar", &texType, PLANE))
			{
				scene->GetActiveModel()->SetTextureMapKind(PLANE);
			}

			ImGui::SameLine();
			if (ImGui::RadioButton("Cylindrical ", &texType, CYLINDER))
			{
				scene->GetActiveModel()->SetTextureMapKind(CYLINDER);
			}

			ImGui::SameLine();
			if (ImGui::RadioButton("Spherical ", &texType, SPHERE))
			{
				scene->GetActiveModel()->SetTextureMapKind(SPHERE);
			}

			bool normalType = scene->GetActiveModel()->GetNormalMapStatus();
			bool depthType = scene->GetActiveModel()->GetDepthMapStatus();
			if (ImGui::Checkbox("Use normal map", &(normalType)))
			{
				scene->GetActiveModel()->SetNormalMapStatus(normalType);
				if (!normalType) depthType = normalType; // if normal is off turn depth off
			}

			if (ImGui::Checkbox("Use depth map", &(depthType)))
			{
				scene->GetActiveModel()->SetDepthMapStatus(depthType);
				if (depthType) normalType = depthType; // if depth is on turn normal map on
			}

			int colType = scene->GetActiveModel()->GetColorKind();

			if (ImGui::RadioButton("Color", &colType, COLOR))
			{
				scene->GetActiveModel()->SetColorKind(COLOR);
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Texture", &colType, TEXTURE))
			{
				scene->GetActiveModel()->SetColorKind(TEXTURE);
			}
			

			glm::vec3 modelAmbientColor = scene->GetActiveModel()->GetMaterial().ambientColor;
			if (ImGui::ColorEdit3("M Ambient Color", (float*)&modelAmbientColor))
			{
				scene->GetActiveModel()->GetMaterial().ambientColor= modelAmbientColor;
			}

			glm::vec3 modelDiffuseColor = scene->GetActiveModel()->GetMaterial().diffuseColor;
			if (ImGui::ColorEdit3("M Diffuse Color", (float*)&modelDiffuseColor))
			{
				scene->GetActiveModel()->GetMaterial().diffuseColor = modelDiffuseColor;
			}

			glm::vec3 modelSpecularColor = scene->GetActiveModel()->GetMaterial().specularColor;
			if (ImGui::ColorEdit3("M Specular Color", (float*)&modelSpecularColor))
			{
				scene->GetActiveModel()->GetMaterial().specularColor = modelSpecularColor;
			}

			float shininess = scene->GetActiveModel()->GetMaterial().shininess;
			if (ImGui::SliderFloat("Shininess", &shininess, 0.0f, 20.0f))
			{
				scene->GetActiveModel()->GetMaterial().shininess = shininess;
			}


			ImGui::Text("Automated Transformations");

			static float aTranslateVal = 0.0f;
			ImGui::SliderFloat("A Translate Val", &aTranslateVal, -0.01f, 0.01f);


			bool aRotate = scene->GetActiveModel()->GetARotate();
			if (ImGui::Checkbox("Auto Rotate", &aRotate))
			{
				scene->GetActiveModel()->SetARotate(aRotate);
			}

			ImGui::SameLine();

			bool aTranslate = scene->GetActiveModel()->GetATranslate();
			if (ImGui::Checkbox("Auto Translate", &aTranslate))
			{
				scene->GetActiveModel()->SetATranslate(aTranslate);
			}



			ImGui::Checkbox("Start Scene", &startScene); //toggle scene animation

			for (int i = 0; i < scene->GetModelCount() && startScene; i++) //Automatioc transformations
			{
				if (scene->GetModel(i)->GetARotate())
				{
					scene->GetModel(i)->RotateYModel(glm::radians(0.025f));
				}

				if (scene->GetModel(i)->GetATranslate())
				{
					scene->GetModel(i)->TranslateModel(glm::vec3(aTranslateVal, 0.0f, 0.0f));
				}
			}

			ImGui::Text("Manual Transformations");
			//Axis to work with
			ImGui::RadioButton("x", &axis, X);
			ImGui::SameLine();
			ImGui::RadioButton("y", &axis, Y);
			ImGui::SameLine();
			ImGui::RadioButton("z", &axis, Z);

			if (ImGui::SliderFloat("Translation", &translation, -0.02, 0.02))
			{
				if (axis == X)
				{
					scene->GetActiveModel()->TranslateModel(glm::vec3(translation, 0, 0));
				}
				else if (axis == Y)
				{
					scene->GetActiveModel()->TranslateModel(glm::vec3(0, translation, 0));
				}
				else // Z
				{
					scene->GetActiveModel()->TranslateModel(glm::vec3(0, 0, translation));
				}
				translation = 0.0f;
			}

			

			if (ImGui::SliderFloat("Rotation", &rotation, -2.0f, 2.0f))
			{
				if (axis == X)
				{
					scene->GetActiveModel()->RotateXModel(glm::radians(rotation));
				}
				else if (axis == Y)
				{
					scene->GetActiveModel()->RotateYModel(glm::radians(rotation));
				}
				else // Z
				{
					scene->GetActiveModel()->RotateZModel(glm::radians(rotation));
				}
				rotation = 0.0f;
			}

			if (ImGui::SliderFloat("Scale", &scale, 0.99f, 1.01f))
			{
				scene->GetActiveModel()->ScaleModel(scale);
				scale = 1.0f;
			}
			

			std::shared_ptr<MeshModel> meshModel = std::dynamic_pointer_cast<MeshModel>(scene->GetActiveModel());
			if (meshModel)
			{
				//glm::vec4 normalColor = meshModel->GetNormalModel().GetColor();
				//if (ImGui::ColorEdit3("Normal Color", (float*)&normalColor))
				//{
				//	meshModel->GetNormalModel().SetColor(normalColor);
				//}
			}
		}

		if (ImGui::CollapsingHeader("Lights"))
		{
			if (ImGui::Button("Add Light"))
			{
				std::shared_ptr<MeshModel> LightM = Utils::LoadMeshModel("..\\Data\\crate.obj");
				lightMesh.push_back(LightM);
				scene->AddLight(std::make_shared<PointLight>(*LightM, glm::vec3(0, 0, 0)));
			}
			ImGui::SameLine();

			bool toShow = scene->GetLightsStatus();
			if (ImGui::Checkbox("Show Lights", &toShow))
			{
				scene->SetLightStatus(toShow);
			}


			const char** items;
			std::vector<std::string> modelStrings;
			items = new const char* [scene->GetLightCount()];
			for (int i = 0; i < scene->GetLightCount(); i++)
			{
				std::ostringstream s;
				s << "Light "<< i;
				std::string mystring = s.str();
				modelStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetLightCount(); i++)
			{
				items[i] = modelStrings[i].c_str();
			}

			int currentLightIndex = scene->GetActiveLightIndex();
			ImGui::Combo("Active Light", &currentLightIndex, items, scene->GetLightCount());

			if (currentLightIndex != scene->GetActiveLightIndex())
			{
				scene->SetActiveLightIndex(currentLightIndex);
			}

			delete items;

	
			glm::vec3 ambientLightColor = scene->GetActiveLight()->GetAmbientColor();
			if (ImGui::ColorEdit3("L Ambient Color", (float*)&ambientLightColor))
			{
				scene->GetActiveLight()->SetAmbientColor(ambientLightColor);
			}

			float ambientLightIntnsity = scene->GetActiveLight()->GetAmbientIntensity();
			if (ImGui::SliderFloat("L Ambient Intnsity", &ambientLightIntnsity, 0.0f, 1.0f))
			{
				scene->GetActiveLight()->SetAmbientIntensity(ambientLightIntnsity);
			}

			glm::vec3 diffuseLightColor = scene->GetActiveLight()->GetDiffuseColor();
			if (ImGui::ColorEdit3("L Diffuse Color", (float*)&diffuseLightColor))
			{
				scene->GetActiveLight()->SetDiffuseColor(diffuseLightColor);
			}

			float diffuseLightIntnsity = scene->GetActiveLight()->GetDiffuseIntensity();
			if (ImGui::SliderFloat("L Diffuse Intnsity", &diffuseLightIntnsity, 0.0f, 1.0f))
			{
				scene->GetActiveLight()->SetDiffuseIntensity(diffuseLightIntnsity);
			}

			glm::vec3 specualrLightColor = scene->GetActiveLight()->GetSpecularColor();
			if (ImGui::ColorEdit3("L Specular Color", (float*)&specualrLightColor))
			{
				scene->GetActiveLight()->SetSpecularColor(specualrLightColor);
			}

			float specularLightIntnsity = scene->GetActiveLight()->GetSpecularIntensity();
			if (ImGui::SliderFloat("L Specular Intnsity", &specularLightIntnsity, 0.0f, 1.0f))
			{
				scene->GetActiveLight()->SetSpecularIntensity(specularLightIntnsity);
			}

			//Axis to work with
			ImGui::RadioButton("lx", &lightAxis, X);
			ImGui::SameLine();
			ImGui::RadioButton("ly", &lightAxis, Y);
			ImGui::SameLine();
			ImGui::RadioButton("lz", &lightAxis, Z);

			if (ImGui::SliderFloat("Light Translation", &lightTranslation, -0.02, 0.02))
			{
				if (lightAxis == X)
				{
					scene->GetActiveLight()->TranslateModel(glm::vec3(lightTranslation, 0, 0));
				}
				else if (lightAxis == Y)
				{
					scene->GetActiveLight()->TranslateModel(glm::vec3(0, lightTranslation, 0));
				}
				else // Z
				{
					scene->GetActiveLight()->TranslateModel(glm::vec3(0, 0, lightTranslation));
				}
				lightTranslation = 0.0f;
			}

		}



		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene->AddModel(Utils::LoadMeshModel(outPath));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}