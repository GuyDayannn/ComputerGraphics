#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

#include <iostream>

/**
 * Fields
 */
float STARTSCALE = 1.0f;
bool show_demo_window = false;
bool show_camera_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec3 transAxis = glm::vec3(0.0f, 0.0f, 0.0f);
std::vector< glm::vec3> modelAdditions;
std::vector<float> modelScale;
std::vector<bool> modelAxisWorld;
std::vector<bool> modelAxisModel;
std::vector<bool> faceNormals;
std::vector<bool> vertexNormals;
std::vector<bool> boundingBox;
std::vector< glm::vec3> worldAdditions;
std::vector<float> worldScale;
/**
* Fields for controling model transformations
*/
static float f = 0.0f;
static int counter = 0;
static int degreesY = 0;
static int degreesX = 0;
static int degreesZ = 0;
static float scaleAddition = 1.0f;
static float xyzAddition[3] = { 0.0f, 0.0f, 0.0f }; //xyz addition
static float xyzWorld[3] = { 0.0f, 0.0f, 0.0f };
static float scaleWorld = 1.0f;
int modelCount;
static const char* choosenModel = nullptr;
static const char* choosenFrame = nullptr;
static int active_model_index = 0;
static int world_model_choice = 1;
static bool showAxisWorld = false;
static bool showAxisModel = false;
static bool showFaceNormals = false;
static bool showVertexNormals = false;
static bool showBoundingBox = false;
/**
* Fields for controling camera
*/
int windowWidth = 1280, windowHeight = 720;
int camCount;
static int active_camera_index = 0;
static const char* choosenCam = nullptr;
static float up = windowWidth / 2.0f;
static float down = -(windowHeight / 2.0f);
static float left = -(windowWidth / 2.0f);
static float right = windowHeight / 2.0f;
static float nearZ = 0.0f;
static float farZ = 500.0f;
static float upDown[2] = { windowHeight / 2.0f , -(windowHeight / 2.0f) };
static float leftRight[2] = { -(windowWidth / 2.0f) , windowWidth / 2.0f };
static float nearFar[2] = { 30.0f , 500.0f };
static float cameraPos[3] = { 0.0f,0.0f,3.0f };
static float lookAtPos[3] = { 0.0f,0.0f,0.0f };
static float upPos[3] = { 0.0f,1.0f,0.f };
static float fovy = 100.0f;
std::vector<glm::vec2> upDownVec;
std::vector<glm::vec2> leftRightVec;
std::vector<glm::vec2> nearFarVec;
std::vector<glm::vec3> cameraPosVec;
std::vector<glm::vec3> lookAtPosVec;
std::vector<glm::vec3> upPosVec;
static bool perspectiveProj = false;
std::vector<bool> perspectiveProjVec;
std::vector<float> fovyVec;
static int cam_window_width = windowWidth;
static int cam_window_height = windowHeight;
std::vector<int> cam_window_widthVec;
std::vector<int> cam_window_heightVec;
static float camxyzAddition[3] = { 0.0f, 0.0f, 0.0f }; //xyz addition
static float camxyzWorld[3] = { 0.0f, 0.0f, 0.0f };
static float camscaleAddition = 1.0f;
static float camscaleWorld = 1.0f;
std::vector< glm::vec3> cameraModelAdditions;
std::vector< glm::vec3> cameraWorldAdditions;
std::vector<float> cameraModelScale;
std::vector<float> cameraWorldScale;
static int camdegreesY = 0;
static int camdegreesX = 0;
static int camdegreesZ = 0;
static bool camWorld = false;

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	/*
	perspectiveProj = true;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(windowWidth, windowHeight, scene.GetCameraCount());
	cameraPos[0] = camera->GetCameraLookAt()[0][0];
	cameraPos[1] = camera->GetCameraLookAt()[0][1];
	cameraPos[2] = camera->GetCameraLookAt()[0][2];
	scene.AddCamera(camera);
	upDownVec.push_back(glm::vec2(windowHeight / 2.0f, -(windowHeight / 2.0f)));
	leftRightVec.push_back(glm::vec2(-(windowWidth / 2.0f), windowWidth / 2.0f));
	nearFarVec.push_back(glm::vec2(30.0f, 500.0f));
	cameraPosVec.push_back(glm::vec3(0.0f, 0.0f, 3.0f));
	lookAtPosVec.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	upPosVec.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	perspectiveProjVec.push_back(false);
	camera->UpdateProjType(true);
	//camera->UpdateRotationModel(90.f, "y");
	scene.AddCamera(camera);
	
	std::shared_ptr<MeshModel> model = Utils::LoadMeshModel("..\\Data\\demo.obj");
	scene.AddModel(model);
	modelAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelScale.push_back(1.0f);
	modelAxisWorld.push_back(false);
	modelAxisModel.push_back(false);
	faceNormals.push_back(false);
	vertexNormals.push_back(false);
	worldAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	worldScale.push_back(1.0f);
	model->UpdateModelTransformations(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(180.0f, 180.0f, 180.0f),"y", glm::vec3(0.0f, 0.0f, 0.0f));
	*/
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	/**
	* Starting 1b here
	**/
	/**
	* 1b : (1)
	**/
	

	//shared_ptr<MeshModel> myModel = Utils::LoadMeshModel("..\\Data\\bunny.obj");
	//std::string namestr = myModel->GetModelName();
	//char* name = NULL;
	//scene.AddModel(myModel);
	/*
	shared_ptr<MeshModel> firstModel = Utils::LoadMeshModel("..\\Data\\bunny.obj");
	//firstModel->FitToWindow(1280, 720);
	std::vector<glm::vec3> fit = firstModel->FitToWindow(1280, 720);
	glm::vec3 addition = glm::vec3(150, 0, 0);
	firstModel->UpdateModelTransformations(fit[0], glm::vec3(0.0f, 0.0f, 0.0f), "y", fit[1]);
	//firstModel->UpdateModelTransformations(fit[0], glm::vec3(45.0f, 45.0f, 45.0f), "z", fit[1] + addition);

	//Model Translate then World rotate
	//firstModel->UpdateModelTransformations(fit[0], glm::vec3(0.0f, 0.0f, 0.0f), "z", fit[1] + addition);
	//firstModel->UpdateWorldTransformations(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(20.0f, 20.0f, 20.0f), "z", glm::vec3(0, 0, 0));

	//World Translate then Model rotate
	//firstModel->UpdateWorldTransformations(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), "z", addition);
	//firstModel->UpdateModelTransformations(fit[0], glm::vec3(20.0f, 20.0f, 20.0f), "z", fit[1]);


	scene.AddModel(firstModel);
	int faceCounts = firstModel->GetFacesCount();
	*/

	/*
	for (int i = 0; i < faceCounts; i++)
	{
		Face temp_face(firstModel->GetFace(i)); //gets ith Face
		int firstVIn = temp_face.GetVertexIndex(0);	//Gets first vertex's index in this face
		int secondVIn = temp_face.GetVertexIndex(1);
		int thirdVIn = temp_face.GetVertexIndex(2);
		glm::vec3 v1 = firstModel->GetTransformedVertex(firstVIn);
		glm::vec3 v2 = firstModel->GetTransformedVertex(secondVIn);
		glm::vec3 v3 = firstModel->GetTransformedVertex(thirdVIn);
		std::cout << "Face #" << i << ": v1 = (" << v1.x << ", " << v1.y << ", " << v1.z << ")   ";
		std::cout << "v2 = (" << v2.x << ", " << v2.y << ", " << v2.z << ")   ";
		std::cout << "v3 = (" << v3.x << ", " << v3.y << ", " << v3.z << ")" << std::endl;

	}
	*/

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here

		if (io.KeysDown['W'] && modelCount > 0) // World
		{
			// W key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
			choosenFrame = "World";
			world_model_choice = 0;
			world_model_choice = 0;
			xyzAddition[0] = worldAdditions[active_model_index][0];
			xyzAddition[1] = worldAdditions[active_model_index][1];
			xyzAddition[2] = worldAdditions[active_model_index][2];
			scaleAddition = worldScale[active_model_index];
		}
		if (io.KeysDown['M'] && modelCount > 0) // Model
		{
			// M key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
			choosenFrame = "Model";
			world_model_choice = 1;
			xyzAddition[0] = modelAdditions[active_model_index][0];
			xyzAddition[1] = modelAdditions[active_model_index][1];
			xyzAddition[2] = modelAdditions[active_model_index][2];
			scaleAddition = modelScale[active_model_index];
		}

		if (io.KeysDown['C'] && xyzAddition[0] < 640.0f) //C - translation by positive 1 on x
		{
			if (world_model_choice == 1)
				modelAdditions[active_model_index][0] = ++xyzAddition[0];
			else
				worldAdditions[active_model_index][0] = ++xyzAddition[0];
		}

		if (io.KeysDown['Z'] && xyzAddition[0] > -640.0f) //Z - translation by negative 1 on x
		{
			if (world_model_choice == 1)
				modelAdditions[active_model_index][0] = --xyzAddition[0];
			else
				worldAdditions[active_model_index][0] = --xyzAddition[0];
		}

		if (io.KeysDown['S'] && xyzAddition[1] < 360.0f) //S  -  translation by positive 1 on y
		{
			if (world_model_choice == 1)
				modelAdditions[active_model_index][1] = ++xyzAddition[1];
			else
				worldAdditions[active_model_index][1] = ++xyzAddition[1];
		}

		if (io.KeysDown['X'] && xyzAddition[1] > -360.0f) //X -  translation by negative 1 on y
		{
			if (world_model_choice == 1)
				modelAdditions[active_model_index][1] = --xyzAddition[1];
			else
				worldAdditions[active_model_index][1] = --xyzAddition[1];
		}

		if (io.KeysDown['/']) // '/' -rotatin around y +
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE * tempScale, glm::vec3(++degreesY, ++degreesY, ++degreesY), "y", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(++degreesY, ++degreesY, ++degreesY), "y", worldAdditions[i]);
					}
				}

			}
			degreesY = 0;
		}
		if (io.KeysDown[',']) // ',' -rotatin around y-
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE * tempScale, glm::vec3(--degreesY, --degreesY, --degreesY), "y", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(--degreesY, --degreesY, --degreesY), "y", worldAdditions[i]);
					}
				}

			}
			degreesY = 0;
		}
		if (io.KeysDown['L']) // L - roation around X + (up)
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(++degreesX, ++degreesX, ++degreesX), "x", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(++degreesX, ++degreesX, ++degreesX), "x", worldAdditions[i]);
					}
				}
			}
			degreesX = 0;

		}
		if (io.KeysDown['.']) // . - roation around X + (down)
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(--degreesX, --degreesX, --degreesX), "x", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(--degreesX, --degreesX, --degreesX), "x", worldAdditions[i]);
					}
				}
			}
			degreesX = 0;

		}

		if (io.KeysDown['='] && scaleAddition <= 14.0f) // scaling +
		{
			if (world_model_choice == 1)
				modelScale[active_model_index] = ++scaleAddition;
			else
				worldScale[active_model_index] = ++scaleAddition;
			
		}

		if (io.KeysDown['-'] && scaleAddition >= 1.0f) //scaling -
		{
			if (world_model_choice == 1)
				modelScale[active_model_index] = --scaleAddition;
			else
				worldScale[active_model_index] = --scaleAddition;

		}

	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					modelAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
					modelScale.push_back(1.0f);
					modelAxisWorld.push_back(false);
					modelAxisModel.push_back(false);
					faceNormals.push_back(false);
					vertexNormals.push_back(false);
					boundingBox.push_back(false);
					worldAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
					worldScale.push_back(1.0f);
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}

			if (ImGui::MenuItem("Add Camera", "CTRL+C"))
			{
				std::shared_ptr<Camera> camera = std::make_shared<Camera>(windowWidth, windowHeight, scene.GetCameraCount());
				cameraPos[0] = camera->GetCameraLookAt()[0][0];
				cameraPos[1] = camera->GetCameraLookAt()[0][1];
				cameraPos[2] = camera->GetCameraLookAt()[0][2];
				scene.AddCamera(camera);
				upDownVec.push_back(glm::vec2(windowHeight / 2.0f, -(windowHeight / 2.0f)));
				leftRightVec.push_back(glm::vec2(-(windowWidth / 2.0f), windowWidth / 2.0f));
				nearFarVec.push_back(glm::vec2(30.0f, 500.0f));
				cameraPosVec.push_back(glm::vec3(0.0f, 0.0f, 3.0f));
				lookAtPosVec.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				upPosVec.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				perspectiveProjVec.push_back(false);
				fovyVec.push_back(100.0f);
				cam_window_widthVec.push_back(windowWidth);
				cam_window_heightVec.push_back(windowHeight);
				cameraModelAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				cameraWorldAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				cameraModelScale.push_back(1.0f);
				cameraWorldScale.push_back(1.0f);
			}

			
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
		//ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		modelCount = scene.GetModelCount();
		camCount = scene.GetCameraCount();

		if (perspectiveProj == 0)
			STARTSCALE = 200.0f;
		else
			STARTSCALE = 1.0f;

		//translating and scaling object
		if (world_model_choice == 1)
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (i == active_model_index)
				{
					//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
					glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
					scene.GetModel(i).UpdateModelTransformations(STARTSCALE * tempScale, glm::vec3(0.0f, 0.0f, 0.0f), "z", modelAdditions[i]);
				}
			}
		}
		else
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (i == active_model_index)
				{
					glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
					scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(0.0f, 0.0f, 0.0f), "z", worldAdditions[i]);
				}
			}

		}
		

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Checkbox("Camera Window", &show_camera_window);
		//ImGui::Checkbox("Demo", &show_demo_window);

		// TODO: Add more menubar items (if you want to)
		
		//Frames - World or Model
		if (ImGui::BeginCombo("Frame Type", choosenFrame))
		{
			if (ImGui::Selectable("World", choosenFrame == "World") && modelCount > 0)
			{
				choosenFrame = "World";
				world_model_choice = 0;
				xyzAddition[0] = worldAdditions[active_model_index][0];
				xyzAddition[1] = worldAdditions[active_model_index][1];
				xyzAddition[2] = worldAdditions[active_model_index][2];
				scaleAddition = worldScale[active_model_index];
			}
			if (choosenFrame == "World")
				ImGui::SetItemDefaultFocus();


			if (ImGui::Selectable("Model", choosenFrame == "Model") && modelCount > 0)
			{
				choosenFrame = "Model";
				world_model_choice = 1;
				xyzAddition[0] = modelAdditions[active_model_index][0];
				xyzAddition[1] = modelAdditions[active_model_index][1];
				xyzAddition[2] = modelAdditions[active_model_index][2];
				scaleAddition = modelScale[active_model_index];
			}
			if (choosenFrame == "Model")
				ImGui::SetItemDefaultFocus();

			ImGui::EndCombo();

		}

		//Model choosing
		if (ImGui::BeginCombo("Models", choosenModel))
		{
			if (world_model_choice == 1) // changing models just if on model transformation
			{
				for (int i = 0; i < modelCount; i++)
				{
					bool selectedModel = (scene.GetModel(i).GetModelName().c_str() == choosenModel);
					if (ImGui::Selectable(scene.GetModel(i).GetModelName().c_str(), selectedModel)) // scene.GetModel(i).GetModelName().c_str() returns NULL
					{
						choosenModel = scene.GetModel(i).GetModelName().c_str();
						active_model_index = i;
						scene.SetActiveModelIndex(i);
						xyzAddition[0] = modelAdditions[active_model_index][0];
						xyzAddition[1] = modelAdditions[active_model_index][1];
						xyzAddition[2] = modelAdditions[active_model_index][2];
						scaleAddition = modelScale[active_model_index];
						showAxisWorld = modelAxisWorld[active_model_index];
						showAxisModel = modelAxisModel[active_model_index];
						showFaceNormals = faceNormals[active_model_index];
						showVertexNormals = vertexNormals[active_model_index];
						showBoundingBox = boundingBox[active_model_index];
					}
					if (selectedModel)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

		
		if (ImGui::SliderFloat("Scale", &scaleAddition, 0.0f, 15.0f)) //scaling
		{
			if (world_model_choice == 1)
			{
				modelScale[active_model_index] = scaleAddition;
			}
			else
			{
				worldScale[active_model_index] = scaleAddition;
			}
		}
		if (ImGui::SliderFloat3("Translation", xyzAddition, -640.0f, 640.0f)) //translating
		{
			if (world_model_choice == 1)
			{
				modelAdditions[active_model_index][0] = xyzAddition[0];
				modelAdditions[active_model_index][1] = xyzAddition[1];
				modelAdditions[active_model_index][2] = xyzAddition[2];
			}
			else
			{
				worldAdditions[active_model_index][0] = xyzAddition[0];
				worldAdditions[active_model_index][1] = xyzAddition[1];
				worldAdditions[active_model_index][2] = xyzAddition[2];
			}
		}
		
		
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		
		if (ImGui::SliderInt("Rotation around y", &degreesY, -10, 10)) //rotation slider - rotating as long as slider held relesing stop rotation
		{

			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesY, degreesY, degreesY), "y", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(degreesY, degreesY, degreesY), "y", worldAdditions[i]);
					}
				}
				
			}
			degreesY = 0;
		}
		if (ImGui::SliderInt("Rotation around z", &degreesZ, -10, 10)) //rotation slider - rotating as long as slider held relesing stop rotation
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesZ, degreesZ, degreesZ), "z", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(degreesZ, degreesZ, degreesZ), "z", worldAdditions[i]);
					}
				}
			}
			degreesZ = 0;
		}
		if (ImGui::SliderInt("Rotation around x", &degreesX, -10, 10)) //rotation slider - rotating as long as slider held relesing stop rotation
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_model_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesX, degreesX, degreesX), "x", modelAdditions[i]);
					}
				}
				else
				{
					if (i == active_model_index)
					{
						glm::vec3 tempScale = glm::vec3(worldScale[i], worldScale[i], worldScale[i]);
						scene.GetModel(i).UpdateWorldTransformations(tempScale, glm::vec3(degreesX, degreesX, degreesX), "x", worldAdditions[i]);
					}
				}	
			}
			degreesX = 0;
		}


		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);
		
		if (ImGui::Checkbox("Show World Frame Axis", &showAxisWorld) && modelCount != 0)
		{
			modelAxisWorld[active_model_index] = showAxisWorld;
			if (showAxisWorld) scene.GetModel(active_model_index).ShowWorldAxis();
			else scene.GetModel(active_model_index).HideWorldAxis();

		}

		if (ImGui::Checkbox("Show Model Frame Axis", &showAxisModel) && modelCount != 0)
		{
			modelAxisModel[active_model_index] = showAxisModel;
			if (showAxisModel) scene.GetModel(active_model_index).ShowModelAxis();
			else scene.GetModel(active_model_index).HideModelAxis();

		}

		if (ImGui::Checkbox("Show Face Normals", &showFaceNormals) && modelCount != 0)
		{
			faceNormals[active_model_index] = showFaceNormals;
			if (showFaceNormals) scene.GetModel(active_model_index).ShowFaceNormals();
			else scene.GetModel(active_model_index).HideFaceNormals();

		}

		if (ImGui::Checkbox("Show Vertex Normals", &showVertexNormals) && modelCount != 0)
		{
			vertexNormals[active_model_index] = showVertexNormals;
			if (showVertexNormals) scene.GetModel(active_model_index).ShowVertexNormals();
			else scene.GetModel(active_model_index).HideVertexNormals();

		}

		if (ImGui::Checkbox("Show Bounding Box", &showBoundingBox) && modelCount != 0)
		{
			boundingBox[active_model_index] = showBoundingBox;
			if (showBoundingBox) scene.GetModel(active_model_index).displayBoundingBox = showBoundingBox;
			else scene.GetModel(active_model_index).displayBoundingBox = showBoundingBox;

		}


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_camera_window && camCount > 0)
	{
		ImGui::Begin("Camera Window", &show_camera_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		
		if (ImGui::Checkbox("Perspective Projection", &perspectiveProj) && camCount != 0)
		{
			perspectiveProjVec[active_camera_index] = perspectiveProj;
			scene.GetActiveCamera().UpdateProjType(perspectiveProj);
			
			if (perspectiveProj == 0)
				STARTSCALE = 200.0f;
			else
				STARTSCALE = 1.0f; //Now Outside of func
			
			for (int i = 0; i < scene.GetModelCount(); i++)
			{
				scene.GetModel(i).UpdateAxisScale(perspectiveProj);
				glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
				scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesX, degreesX, degreesX), "x", modelAdditions[i]);
			}
		}

		/*
		if (perspectiveProj)
		{
			if (ImGui::SliderInt("Window Width", &cam_window_width, 0, 1280))
			{
				cam_window_widthVec[active_camera_index] = cam_window_width;
				scene.GetCamera(active_camera_index).SetWindowSize(cam_window_widthVec[active_camera_index], cam_window_heightVec[active_camera_index]);
			}

		}

		if (perspectiveProj)
		{
			if (ImGui::SliderInt("Window Height", &cam_window_height, 0, 720))
			{
				cam_window_heightVec[active_camera_index] = cam_window_height;
				scene.GetCamera(active_camera_index).SetWindowSize(cam_window_widthVec[active_camera_index], cam_window_heightVec[active_camera_index]);
			}

		}
		*/
		if (perspectiveProj)
		{
			if (ImGui::SliderFloat("FOV y", &fovy, 45.0f, 120.0f))
			{
				scene.GetCamera(active_camera_index).UpdateViewVolume(upDownVec[active_camera_index][0], upDownVec[active_camera_index][1], leftRightVec[active_camera_index][0], leftRightVec[active_camera_index][1], nearFarVec[active_camera_index][0], nearFarVec[active_camera_index][1], fovy);
				fovyVec[active_camera_index] = fovy;
			}
		}

		
		if (ImGui::BeginCombo("Cameras", choosenCam)) //!!!!!!!!!!!!problem here!!!!!!!!!!!!!!!!
		{
			
			for (int i = 0; i < camCount; i++)
			{
				bool selectedCam = ((scene.GetCamera(i).GetCamName().c_str() == choosenCam));
				if (ImGui::Selectable(scene.GetCamera(i).GetCamName().c_str(), selectedCam)) // scene.GetModel(i).GetModelName().c_str() returns NULL
				{
					active_camera_index = i;
					scene.SetActiveCameraIndex(i);
					choosenCam = scene.GetCamera(i).GetCamName().c_str();
					upDown[0] = upDownVec[i].x;
					upDown[1] = upDownVec[i].y;
					leftRight[0] = leftRightVec[i].x;
					leftRight[1] = leftRightVec[i].y;
					nearFar[0] = nearFarVec[i].x;
					nearFar[1] = nearFarVec[i].y;
					cameraPos[0] = cameraPosVec[i].x;
					cameraPos[1] = cameraPosVec[i].y;
					cameraPos[2] = cameraPosVec[i].z;
					lookAtPos[0] = lookAtPosVec[i].x;
					lookAtPos[1] = lookAtPosVec[i].y;
					lookAtPos[2] = lookAtPosVec[i].z;
					upPos[0] = upPosVec[i].x;
					upPos[1] = upPosVec[i].y;
					upPos[2] = upPosVec[i].z;
					perspectiveProj = perspectiveProjVec[active_camera_index];
					fovy = fovyVec[active_camera_index];

					if (!camWorld)
					{
						camxyzAddition[0] = cameraModelAdditions[active_camera_index][0];
						camxyzAddition[1] = cameraModelAdditions[active_camera_index][1];
						camxyzAddition[2] = cameraModelAdditions[active_camera_index][2];
					}
					else
					{
						camxyzAddition[0] = cameraWorldAdditions[active_camera_index][0];
						camxyzAddition[1] = cameraWorldAdditions[active_camera_index][1];
						camxyzAddition[2] = cameraWorldAdditions[active_camera_index][2];
					}

					
					if (perspectiveProj == 0)
						STARTSCALE = 200.0f;
					else
						STARTSCALE = 1.0f; //Now Outside of func

					for (int i = 0; i < scene.GetModelCount(); i++)
					{
						scene.GetModel(i).UpdateAxisScale(perspectiveProj);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE * tempScale, glm::vec3(degreesX, degreesX, degreesX), "x", modelAdditions[i]);
					}
					
				}
				if (selectedCam)
					ImGui::SetItemDefaultFocus();
				}
			
			ImGui::EndCombo();
		}
		
		if (ImGui::SliderFloat2("Up Down", upDown, -360.0f, 360.0f))
		{
			std::vector<float> leftRight = scene.GetCamera(active_camera_index).GetLeftRightVals();
			std::vector<float> nearFar = scene.GetCamera(active_camera_index).GetNearFarVals();
			scene.GetCamera(active_camera_index).UpdateViewVolume(upDown[0], upDown[1], leftRight[0], leftRight[1], nearFar[0], nearFar[1], fovyVec[active_camera_index]);
			upDownVec[active_camera_index][0] = upDown[0];
			upDownVec[active_camera_index][1] = upDown[1];
			leftRightVec[active_camera_index][0] = leftRight[0];
			leftRightVec[active_camera_index][1] = leftRight[1];
			nearFarVec[active_camera_index][0] = nearFar[0];
			nearFarVec[active_camera_index][1] = nearFar[1];
			cameraPosVec[active_camera_index][0] = cameraPos[0];
			cameraPosVec[active_camera_index][1] = cameraPos[1];
			cameraPosVec[active_camera_index][2] = cameraPos[2];
			lookAtPosVec[active_camera_index][0] = lookAtPos[0];
			lookAtPosVec[active_camera_index][1] = lookAtPos[1];
			lookAtPosVec[active_camera_index][2] = lookAtPos[2];
			upPosVec[active_camera_index][0] = upPos[0];
			upPosVec[active_camera_index][1] = upPos[1];
			upPosVec[active_camera_index][2] = upPos[2];
			//fovyVec[active_camera_index] = fovy;
		}

		if (ImGui::SliderFloat2("Left Right", leftRight, -640.0f, 640.0f))
		{
			std::vector<float> upDown = scene.GetCamera(active_camera_index).GetUpDownVals();
			std::vector<float> nearFar = scene.GetCamera(active_camera_index).GetNearFarVals();
			scene.GetCamera(active_camera_index).UpdateViewVolume(upDown[0], upDown[1], leftRight[0], leftRight[1], nearFar[0], nearFar[1], fovyVec[active_camera_index]);
			upDownVec[active_camera_index][0] = upDown[0];
			upDownVec[active_camera_index][1] = upDown[1];
			leftRightVec[active_camera_index][0] = leftRight[0];
			leftRightVec[active_camera_index][1] = leftRight[1];
			nearFarVec[active_camera_index][0] = nearFar[0];
			nearFarVec[active_camera_index][1] = nearFar[1];
			cameraPosVec[active_camera_index][0] = cameraPos[0];
			cameraPosVec[active_camera_index][1] = cameraPos[1];
			cameraPosVec[active_camera_index][2] = cameraPos[2];
			lookAtPosVec[active_camera_index][0] = lookAtPos[0];
			lookAtPosVec[active_camera_index][1] = lookAtPos[1];
			lookAtPosVec[active_camera_index][2] = lookAtPos[2];
			upPosVec[active_camera_index][0] = upPos[0];
			upPosVec[active_camera_index][1] = upPos[1];
			upPosVec[active_camera_index][2] = upPos[2];
			//fovyVec[active_camera_index] = fovy;
		}

		if (ImGui::SliderFloat2("Near Far", nearFar, 30.0f, 500.0f))
		{
			std::vector<float> upDown = scene.GetCamera(active_camera_index).GetUpDownVals();
			std::vector<float> leftRight = scene.GetCamera(active_camera_index).GetLeftRightVals();
			scene.GetCamera(active_camera_index).UpdateViewVolume(upDown[0], upDown[1], leftRight[0], leftRight[1], nearFar[0], nearFar[1], fovyVec[active_camera_index]);
			upDownVec[active_camera_index][0] = upDown[0];
			upDownVec[active_camera_index][1] = upDown[1];
			leftRightVec[active_camera_index][0] = leftRight[0];
			leftRightVec[active_camera_index][1] = leftRight[1];
			nearFarVec[active_camera_index][0] = nearFar[0];
			nearFarVec[active_camera_index][1] = nearFar[1];
			cameraPosVec[active_camera_index][0] = cameraPos[0];
			cameraPosVec[active_camera_index][1] = cameraPos[1];
			cameraPosVec[active_camera_index][2] = cameraPos[2];
			lookAtPosVec[active_camera_index][0] = lookAtPos[0];
			lookAtPosVec[active_camera_index][1] = lookAtPos[1];
			lookAtPosVec[active_camera_index][2] = lookAtPos[2];
			upPosVec[active_camera_index][0] = upPos[0];
			upPosVec[active_camera_index][1] = upPos[1];
			upPosVec[active_camera_index][2] = upPos[2];
			//fovyVec[active_camera_index] = fovy;
		}


		if (ImGui::SliderFloat3("Camera Position xyz", cameraPos, -10.0f, 10.0f))
		{
			std::vector<glm::vec3> prop = scene.GetCamera(active_camera_index).GetCameraLookAt();
			scene.GetCamera(active_camera_index).SetCameraLookAt(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]), prop[1], prop[2]);
			upDownVec[active_camera_index][0] = upDown[0];
			upDownVec[active_camera_index][1] = upDown[1];
			leftRightVec[active_camera_index][0] = leftRight[0];
			leftRightVec[active_camera_index][1] = leftRight[1];
			nearFarVec[active_camera_index][0] = nearFar[0];
			nearFarVec[active_camera_index][1] = nearFar[1];
			cameraPosVec[active_camera_index][0] = cameraPos[0];
			cameraPosVec[active_camera_index][1] = cameraPos[1];
			cameraPosVec[active_camera_index][2] = cameraPos[2];
			lookAtPosVec[active_camera_index][0] = lookAtPos[0];
			lookAtPosVec[active_camera_index][1] = lookAtPos[1];
			lookAtPosVec[active_camera_index][2] = lookAtPos[2];
			upPosVec[active_camera_index][0] = upPos[0];
			upPosVec[active_camera_index][1] = upPos[1];
			upPosVec[active_camera_index][2] = upPos[2];
			//fovyVec[active_camera_index] = fovy;
		}

		if (ImGui::RadioButton("Local", !camWorld))
		{
			camWorld = false;
			camxyzAddition[0] = cameraModelAdditions[active_camera_index][0];
			camxyzAddition[1] = cameraModelAdditions[active_camera_index][1];
			camxyzAddition[2] = cameraModelAdditions[active_camera_index][2];
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("World", camWorld))
		{
			camWorld = true;
			camxyzAddition[0] = cameraWorldAdditions[active_camera_index][0];
			camxyzAddition[1] = cameraWorldAdditions[active_camera_index][1];
			camxyzAddition[2] = cameraWorldAdditions[active_camera_index][2];
		}

		if (ImGui::SliderFloat3("Translation", camxyzAddition, -10.0f, 10.0f))
		{
			if (!camWorld)
			{
				cameraModelAdditions[active_camera_index] = glm::vec3(camxyzAddition[0], camxyzAddition[1], camxyzAddition[2]);
				scene.GetCamera(active_camera_index).UpdateTranslationModel(cameraModelAdditions[active_camera_index]);
			}
			else
			{
				cameraWorldAdditions[active_camera_index] = glm::vec3(camxyzAddition[0], camxyzAddition[1], camxyzAddition[2]);
				scene.GetCamera(active_camera_index).UpdateTranslationWorld(cameraWorldAdditions[active_camera_index]);
			}
		}

		if (ImGui::SliderInt("Rotation", &camdegreesY, -10.0f, 10.0f))
		{
			if (!camWorld)
			{
				scene.GetCamera(active_camera_index).UpdateRotationModel(camdegreesY, "y");
				camdegreesY = 0.0f;
			}
			else
			{
				scene.GetCamera(active_camera_index).UpdateRotationWorld(camdegreesY, "y");
				camdegreesY = 0.0f;
			}

		}

															   
		//ImGui::Text("Hello from another window!");
		//if (ImGui::Button("Close Me"))
			//show_camera_window = false;
		ImGui::End();
	}
}