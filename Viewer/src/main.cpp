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
bool show_demo_window = false;
bool show_another_window = false;
bool scale = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

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

int windowWidth = 1280, windowHeight = 720;
int main(int argc, char** argv)
{
	//int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	scene.AddCamera(std::make_shared<Camera>());
	scene.GetCamera(0).left = -windowWidth;
	scene.GetCamera(0).right = windowWidth;
	scene.GetCamera(0).down = -windowHeight;
	scene.GetCamera(0).up = windowHeight;

	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
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

			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)

		//ImGui::SliderFloat("X", &scene.GetModel(scene.GetActiveModelIndex()).translatex, -680, 680);
		//ImGui::SliderFloat("Y", &scene.GetModel(scene.GetActiveModelIndex()).translatey, -340, 340);
		if (io.KeysDown[262])
		{
			 scene.GetCamera(0).rotate -= 10;
		}
		if (io.KeysDown[263])
		{
			scene.GetCamera(0).rotate += 10;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			scene.GetCamera(0).translatex -= 10;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			scene.GetCamera(0).translatex += 10;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			scene.GetCamera(0).translatey -= 10;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			scene.GetCamera(0).translatey += 10;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			scene.GetCamera(0).translatez += 10;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			scene.GetCamera(0).translatez -= 10;
		}
	}


	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (scene.GetModelCount())
		{
			auto mouse = ImGui::GetMouseDragDelta();
			scene.GetCamera(0).translatex += mouse.x / 50;
			scene.GetCamera(0).translatey -= mouse.y / 50;
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

bool local = false;
bool world = false;
bool change = false;
bool orthoProjectionControl = false;
bool PerspictiveProjectionControl = false;
bool localcam = false;
bool worldcam = false;
bool dolly = false;

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	//ImGui::Begin("MeshViewer Menu");

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

		if (ImGui::BeginMenu("Mesh"))
		{
			if (ImGui::MenuItem("Local transformations"))
				local = true;
			if (ImGui::MenuItem("World transformations"))
				world = true;
			if (ImGui::MenuItem("select model"))
				change = true;
			ImGui::Checkbox("Bounding Box", &scene.bounding);
			ImGui::Checkbox("Show axis", &scene.axis);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Camera"))
		{
			if (ImGui::MenuItem("local camera transformation"))
				localcam = true;
			if (ImGui::MenuItem("world camera transformation"))
				worldcam = true;
			if (ImGui::MenuItem("Ortho Projection"))
				orthoProjectionControl = true;
			if (ImGui::MenuItem("Perspictive Projection"))
				PerspictiveProjectionControl = true;
			if (ImGui::MenuItem("dolly zoom"))
				dolly = true;
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	//ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	//ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);



	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//}

	if(change == 1)
	{
		static int active = 0;
		static const char* models[] = { "model 1","model 2", "model 3", "model 4", "model 5", "model 6", "model 7",
		"model 8", "model 9", "model 10" };
		ImGui::Begin("Active model");
		ImGui::ListBox("models", &active, models, scene.GetModelCount());
		scene.SetActiveModelIndex(active);
		if (ImGui::Button("Close Me"))
			change = false;
		ImGui::End();
	}

	if (orthoProjectionControl == 1)
	{
		static float n = 0.01f;
		static float f = 100.0f;
		ImGui::Begin("orho projection control");
		ImGui::SliderFloat("left", &scene.GetCamera(0).left, -windowWidth*4, windowWidth*2);
		ImGui::SliderFloat("right", &scene.GetCamera(0).right, -windowWidth * 2, windowWidth * 4);
		ImGui::SliderFloat("down", &scene.GetCamera(0).down, -windowHeight * 4, windowHeight * 2);
		ImGui::SliderFloat("up", &scene.GetCamera(0).up, -windowHeight * 2, windowHeight * 4);
		ImGui::SliderFloat("near", &scene.GetCamera(0).near2, 0.0f, 100.0f);
		ImGui::SliderFloat("far", &scene.GetCamera(0).far2, 0.0f, 100.0f);
		if (ImGui::Button("Close Me"))
			orthoProjectionControl = false;
		ImGui::End();
	}

	if (dolly == 1)
	{
		static float f = 0.0f;
		ImGui::Begin("orho projection control");
		ImGui::SliderFloat("left", &f, 0, 1.0f);
		scene.GetCamera(0).down = -windowHeight - 1000 *f;
		scene.GetCamera(0).up = windowHeight + 1000 *f;
		scene.GetCamera(0).left = -windowWidth - 1000 *f;
		scene.GetCamera(0).right = windowWidth + 1000 *f;
		scene.GetCamera(0).near2 = 1.0f + f;

		if (ImGui::Button("Close Me"))
			dolly = false;
		ImGui::End();
	} 

	if (PerspictiveProjectionControl == 1)
	{
		static float n = 0.01f;
		static float f = 100.0f;
		ImGui::Begin("Perspictive projection control");
		ImGui::SliderFloat("left", &scene.GetCamera(0).left, -windowWidth * 4, windowWidth * 2);
		ImGui::SliderFloat("right", &scene.GetCamera(0).right, -windowWidth * 2, windowWidth * 4);
		ImGui::SliderFloat("down", &scene.GetCamera(0).down, -windowHeight * 4, windowHeight * 2);
		ImGui::SliderFloat("up", &scene.GetCamera(0).up, -windowHeight * 2, windowHeight * 4);
		ImGui::SliderFloat("near", &scene.GetCamera(0).near2, 0.0f, 100.0f);
		ImGui::SliderFloat("far", &scene.GetCamera(0).far2, 0.0f, 100.0f);
		if (ImGui::Button("Close Me"))
			orthoProjectionControl = false;
		ImGui::End();
	}

	if (localcam)
	{
		ImGui::Begin("Local camera transformations");
		static float f = 1.0f;
		ImGui::SliderFloat("X", &scene.GetCamera(0).translatex, -680, 680);
		ImGui::SliderFloat("Y", &scene.GetCamera(0).translatey, -340, 340);
		ImGui::SliderFloat("Z", &scene.GetCamera(0).translatez, -340, 340);
		ImGui::SliderFloat("rotate", &scene.GetCamera(0).rotate, -360.0f, 360.0f);
		if (ImGui::Button("Close Me"))
			localcam = false;
		ImGui::End();
	}

	if (worldcam)
	{
		ImGui::Begin("Lworld camera transformations");
		static float f = 1.0f;
		ImGui::SliderFloat("X", &scene.GetCamera(0).Wtranslatex, -680, 680);
		ImGui::SliderFloat("Y", &scene.GetCamera(0).Wtranslatey, -340, 340);
		ImGui::SliderFloat("Z", &scene.GetCamera(0).Wtranslatez, -340, 340);
		ImGui::SliderFloat("rotate", &scene.GetCamera(0).Wrotate, -360.0f, 360.0f);
		if (ImGui::Button("Close Me"))
			worldcam = false;
		ImGui::End();
	}

	if(local == 1)
	{
		static float f = 1.0f;

		ImGui::Begin("Local transformations");
		static int e = 0;
		ImGui::RadioButton("scale", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("translate", &e, 1); ImGui::SameLine();
		ImGui::RadioButton("rotate", &e, 2);
		
		if (e == 0)
		{
			if (scene.GetModelCount() > 0)
			{
				ImGui::SliderFloat("X", &scene.GetModel(scene.GetActiveModelIndex()).scalex, 0, 2.0f);
				ImGui::SliderFloat("Y", &scene.GetModel(scene.GetActiveModelIndex()).scaley, 0, 2.0f);
				ImGui::SliderFloat("Z", &f, 0, 2.0f);
			}
		}
		if (e == 1)
		{
			if (scene.GetModelCount() > 0)
			{
				ImGui::SliderFloat("X", &scene.GetModel(scene.GetActiveModelIndex()).translatex, -680, 680);
				ImGui::SliderFloat("Y", &scene.GetModel(scene.GetActiveModelIndex()).translatey, -340, 340);
				ImGui::SliderFloat("Z", &f, -340, 340);
			}
		}
		if (e == 2)
		{
			if (scene.GetModelCount() > 0)
			{
				//ImGui::SliderFloat("X", &scene.GetModel(0).translatex, -680, 680);
				ImGui::SliderFloat("rotate", &scene.GetModel(scene.GetActiveModelIndex()).rotate, -360.0f, 360.0f);
			}
		}
		if (ImGui::Button("Close Me"))
			local = false;
		ImGui::End();
	}

	if(world)
	{
		static float f = 1.0f;
		ImGui::Begin("World transformations");
		static int e = 0;
		ImGui::RadioButton("scale", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("translate", &e, 1); ImGui::SameLine();
		ImGui::RadioButton("rotate", &e, 2);
		if (e == 0)
		{
			if (scene.GetModelCount() > 0)
			{
				ImGui::SliderFloat("X", &scene.GetModel(scene.GetActiveModelIndex()).Wscalex, 0, 2.0f);
				ImGui::SliderFloat("Y", &scene.GetModel(scene.GetActiveModelIndex()).Wscaley, 0, 2.0f);
				ImGui::SliderFloat("Z", &f, 0, 2.0f);
			}
		}
		if (e == 1)
		{
			if (scene.GetModelCount() > 0)
			{
				ImGui::SliderFloat("X", &scene.GetModel(scene.GetActiveModelIndex()).Wtranslatex, -680, 680);
				ImGui::SliderFloat("Y", &scene.GetModel(scene.GetActiveModelIndex()).Wtranslatey, -340, 340);
				ImGui::SliderFloat("Z", &f, -340, 340);
			}
		}
		if (e == 2)
		{
			if (scene.GetModelCount() > 0)
			{
				//ImGui::SliderFloat("X", &scene.GetModel(0).translatex, -680, 680);
				ImGui::SliderFloat("rotate", &scene.GetModel(scene.GetActiveModelIndex()).Wrotate, -360.0f, 360.0f);
			}
		}
		if (ImGui::Button("Close Me"))
			world = false;
		ImGui::End();
	}


	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}