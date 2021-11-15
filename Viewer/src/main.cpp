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
#define STARTSCALE 200.0f

/**
 * Fields
 */
bool show_demo_window = false;
bool show_camera_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
glm::vec3 transAxis = glm::vec3(0.0f, 0.0f, 0.0f);
std::vector< glm::vec3> modelAdditions;
std::vector<float> modelScale;
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
static int active_index = 0;
static int world_model_choice = 1;
/**
* Fields for controling camera
*/
float view_width = 1280.0f;
float view_height = 720.0f;
static float up = view_height / 2.0f;
static float down = -(view_height / 2.0f);
static float left = -(view_width / 2.0f);
static float right = view_width / 2.0f;
static float nearZ = 0.0f;
static float farZ = 500.0f;
static float upDown[2] = { view_height / 2.0f , -(view_height / 2.0f) };
static float leftRight[2] = { -(view_width / 2.0f) , view_width / 2.0f };
static float nearFar[2] = { 30.0f , 500.0f };
static float cameraPos[3] = { 0.0f,0.0f,1.0f };
static float lookAtPos[3] = { 0.0f,0.0f,0.0f };
static float upPos[3] = { 0.0f,1.0f,0.f };


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
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	scene.AddCamera(camera);
	//std::shared_ptr<MeshModel> model = Utils::LoadMeshModel("..\\Data\\bunny.obj");
	//scene.AddModel(model);
	//modelAdditions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	//modelScale.push_back(200.0f);
	//model->UpdateModelTransformations(glm::vec3(200.0f, 200.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f),"x", glm::vec3(640.0f, 300.0f, 0.0f));

	
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
			xyzAddition[0] = xyzWorld[0];
			xyzAddition[1] = xyzWorld[1];
			xyzAddition[2] = xyzWorld[2];
			scaleAddition = scaleWorld;
		}
		if (io.KeysDown['M'] && modelCount > 0) // Model
		{
			// M key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
			choosenFrame = "Model";
			world_model_choice = 1;
			xyzAddition[0] = modelAdditions[active_index][0];
			xyzAddition[1] = modelAdditions[active_index][1];
			xyzAddition[2] = modelAdditions[active_index][2];
			scaleAddition = modelScale[active_index];
		}

		if (io.KeysDown['C'] && xyzAddition[0] < 640.0f) //C - translation by positive 1 on x
		{
			if (world_model_choice == 1)
				modelAdditions[active_index][0] = ++xyzAddition[0];
			else
				xyzWorld[0] = ++xyzAddition[0];
		}

		if (io.KeysDown['Z'] && xyzAddition[0] > -640.0f) //Z - translation by negative 1 on x
		{
			if (world_model_choice == 1)
				modelAdditions[active_index][0] = --xyzAddition[0];
			else
				xyzWorld[0] = --xyzAddition[0];
		}

		if (io.KeysDown['S'] && xyzAddition[1] < 360.0f) //S  -  translation by positive 1 on y
		{
			if (world_model_choice == 1)
				modelAdditions[active_index][1] = ++xyzAddition[1];
			else
				xyzWorld[1] = ++xyzAddition[1];	
		}

		if (io.KeysDown['X'] && xyzAddition[1] > -360.0f) //X -  translation by negative 1 on y
		{
			if (world_model_choice == 1)
				modelAdditions[active_index][1] = --xyzAddition[1];
			else
				xyzWorld[1] = --xyzAddition[1];	
		}

		if (io.KeysDown['/']) // '/' -rotatin around y +
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE * tempScale, glm::vec3(++degreesY, ++degreesY, ++degreesY), "y", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(++degreesY, ++degreesY, ++degreesY), "y", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
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
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE * tempScale, glm::vec3(--degreesY, --degreesY, --degreesY), "y", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(--degreesY, --degreesY, --degreesY), "y", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
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
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(++degreesX, ++degreesX, ++degreesX), "x", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(++degreesX, ++degreesX, ++degreesX), "x", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
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
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(--degreesX, --degreesX, --degreesX), "x", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(--degreesX, --degreesX, --degreesX), "x", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
				}
			}
			degreesX = 0;

		}

		if (io.KeysDown['='] && scaleAddition <= 14.0f) // scaling +
		{
			if (world_model_choice == 1)
				modelScale[active_index] = ++scaleAddition;
			else
				scaleWorld = ++scaleAddition;
			
		}

		if (io.KeysDown['-'] && scaleAddition >= 1.0f) //scaling -
		{
			if (world_model_choice == 1)
				modelScale[active_index] = --scaleAddition;
			else
				scaleWorld = --scaleAddition;	

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
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

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

		//translating and scaling object
		if (world_model_choice == 1)
		{
			for (int i = 0; i < modelCount; i++)
			{
				if (i == active_index)
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
				scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(0.0f, 0.0f, 0.0f), "z", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));	
			}

		}
		

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		

		// TODO: Add more menubar items (if you want to)
		
		//Frames - World or Model
		if (ImGui::BeginCombo("Frame Type", choosenFrame))
		{
			if (ImGui::Selectable("World", choosenFrame == "World") && modelCount > 0)
			{
				choosenFrame = "World";
				world_model_choice = 0;
				xyzAddition[0] = xyzWorld[0];
				xyzAddition[1] = xyzWorld[1];
				xyzAddition[2] = xyzWorld[2];
				scaleAddition = scaleWorld;
			}
			if (choosenFrame == "World")
				ImGui::SetItemDefaultFocus();


			if (ImGui::Selectable("Model", choosenFrame == "Model") && modelCount > 0)
			{
				choosenFrame = "Model";
				world_model_choice = 1;
				xyzAddition[0] = modelAdditions[active_index][0];
				xyzAddition[1] = modelAdditions[active_index][1];
				xyzAddition[2] = modelAdditions[active_index][2];
				scaleAddition = modelScale[active_index];
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
						active_index = i;
						xyzAddition[0] = modelAdditions[active_index][0];
						xyzAddition[1] = modelAdditions[active_index][1];
						xyzAddition[2] = modelAdditions[active_index][2];
						scaleAddition = modelScale[active_index];
					}
					if (selectedModel)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Camera Window", &show_camera_window);

		
		if (ImGui::SliderFloat("Scale", &scaleAddition, 0.0f, 15.0f)) //scaling
		{
			if (world_model_choice == 1)
			{
				modelScale[active_index] = scaleAddition;
			}
			else
			{
				scaleWorld = scaleAddition;
			}
		}
		if (ImGui::SliderFloat3("Translation", xyzAddition, -640.0f, 640.0f)) //translating
		{
			if (world_model_choice == 1)
			{
				modelAdditions[active_index][0] = xyzAddition[0];
				modelAdditions[active_index][1] = xyzAddition[1];
				modelAdditions[active_index][2] = xyzAddition[2];
			}
			else
			{
				xyzWorld[0] = xyzAddition[0];
				xyzWorld[1] = xyzAddition[1];
				xyzWorld[2] = xyzAddition[2];
			}
		}
		
		
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		
		if (ImGui::SliderInt("Rotation around y", &degreesY, -10, 10)) //rotation slider - rotating as long as slider held relesing stop rotation
		{

			for (int i = 0; i < modelCount; i++)
			{
				if (world_model_choice == 1)
				{
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesY, degreesY, degreesY), "y", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(degreesY, degreesY, degreesY), "y", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
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
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesZ, degreesZ, degreesZ), "z", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(degreesZ, degreesZ, degreesZ), "z", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
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
					if (i == active_index)
					{
						//std::vector<glm::vec3> fitV = scene.GetModel(i).FitToWindow(1280, 720);
						glm::vec3 tempScale = glm::vec3(modelScale[i], modelScale[i], modelScale[i]);
						scene.GetModel(i).UpdateModelTransformations(STARTSCALE* tempScale, glm::vec3(degreesX, degreesX, degreesX), "x", modelAdditions[i]);
					}
				}
				else
				{
					scene.GetModel(i).UpdateWorldTransformations(glm::vec3(scaleWorld, scaleWorld, scaleWorld), glm::vec3(degreesX, degreesX, degreesX), "x", glm::vec3(xyzAddition[0], xyzAddition[1], xyzAddition[2]));
				}	
			}
			degreesX = 0;
		}


		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_camera_window)
	{
		ImGui::Begin("Camera Window", &show_camera_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		if (ImGui::SliderFloat2("Up Down", upDown, -360.0f, 360.0f))
		{
			std::vector<float> leftRight = scene.GetCamera(0).GetLeftRightVals();
			std::vector<float> nearFar = scene.GetCamera(0).GetNearFarVals();
			scene.GetCamera(0).UpdateViewVolume(upDown[0], upDown[1], leftRight[0], leftRight[1], nearFar[0], nearFar[1]);
		}

		if (ImGui::SliderFloat2("Left Right", leftRight, -640.0f, 640.0f))
		{
			std::vector<float> upDown = scene.GetCamera(0).GetUpDownVals();
			std::vector<float> nearFar = scene.GetCamera(0).GetNearFarVals();
			scene.GetCamera(0).UpdateViewVolume(upDown[0], upDown[1], leftRight[0], leftRight[1], nearFar[0], nearFar[1]);
		}

		if (ImGui::SliderFloat2("Near Far", nearFar, 0.0f, 500.0f))
		{
			std::vector<float> upDown = scene.GetCamera(0).GetUpDownVals();
			std::vector<float> leftRight = scene.GetCamera(0).GetLeftRightVals();
			scene.GetCamera(0).UpdateViewVolume(upDown[0], upDown[1], leftRight[0], leftRight[1], nearFar[0], nearFar[1]);
		}


		if (ImGui::SliderFloat3("Camera Position xyz", cameraPos, -3.0f, 3.0f))
		{
			std::vector<glm::vec3> prop = scene.GetCamera(0).GetCameraLookAt();
			scene.GetCamera(0).SetCameraLookAt(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]), prop[1], prop[2]);
		}
		
															   
		//ImGui::Text("Hello from another window!");
		//if (ImGui::Button("Close Me"))
			//show_camera_window = false;
		ImGui::End();
	}
}