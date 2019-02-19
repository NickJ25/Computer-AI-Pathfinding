
#include "vendors/imgui.h"
#include "vendors/imgui_impl_glfw.h"
#include "vendors/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>  
#include <GLFW/glfw3.h> 
#include "Graph.h"

using namespace std;
bool show_about_window = false;
bool show_settings_window = false;
static float cameraSpeed = 4.0f;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


Graph* mainGraph;

void init() {
	mainGraph = new Graph("random64_4_1485816605.dot");
}

void update() {

}

void draw(glm::mat4 view) {
	mainGraph->draw(view);
}

void menuDraw() {
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);

	// Options Flags
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(240, 140), ImGuiCond_Once);

	// Options
	ImGui::Begin("Pathfinding Options", NULL, window_flags);

	// UI
	ImGui::PushItemWidth(100);
	const char* items[] = { "A*", "IDA*", "Dijkstra's" };
	static int item_current = 0;
	ImGui::Combo("Algorithm", &item_current, items, IM_ARRAYSIZE(items));

	// Start Node
	static int startNode = 1;
	if (startNode < 0) startNode = mainGraph->getVertexCount();
	else if (startNode > mainGraph->getVertexCount()) startNode = 0;
	ImGui::PushItemWidth(100);
	ImGui::InputInt("Start Node", &startNode);

	// End Node
	static int endNode = 2;
	if (endNode < 0) endNode = mainGraph->getVertexCount();
	else if (endNode > mainGraph->getVertexCount()) endNode = 0;
	ImGui::PushItemWidth(100);
	ImGui::InputInt("End Node", &endNode);

	// Find Button
	if (ImGui::Button("Find")) {
		mainGraph->find(Graph::A_STAR, startNode, endNode);
	}
	ImGui::SameLine();
	// About Button
	if (ImGui::Button("About")) {
		if(!show_about_window)
			show_about_window = true;
		else
			show_about_window = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Settings")) {
		if (!show_settings_window)
			show_settings_window = true;
		else
			show_settings_window = false;
	}

	ImGui::Text("avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	// About window
	if (show_about_window)
	{
		//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(240, 140), ImGuiCond_Once);
		ImGui::Begin("About", &show_about_window, window_flags); 
		ImGui::Text("A simple pathfinding showcase \ncreated by Niclas Jonsson and \nDavid Walbancke. \n\nThis project uses:\n-dear imgui (v1.67)\n-glew \n-glfw");
		ImGui::End();
	}

	if (show_settings_window)
	{
		ImGui::SetNextWindowSize(ImVec2(330, 140), ImGuiCond_Once);
		ImGui::Begin("Settings", &show_about_window, window_flags);  
		ImGui::Text("Camera Options");
		ImGui::InputFloat("Move Increment", &cameraSpeed, 0.5f, 1.0f, "%.1f");
		ImGui::End();
	}

}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer AI - Pathfinder", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);

	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	init();

	ImVec2 prevPos;
	ImVec2 currentPos;
	bool drag = false;
	bool currentDragging = false;
	float xPos = 0;
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		if (ImGui::IsKeyPressed(GLFW_KEY_Q, true)) {
			mainGraph->scale(1.1);
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_E, true)) {
			mainGraph->scale(0.9);
		}

		if (ImGui::IsKeyPressed(GLFW_KEY_A, true)) {
			view = glm::translate(view, glm::vec3(cameraSpeed, 0.0, 0.0));
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_D, true)) {
			view = glm::translate(view, glm::vec3(-cameraSpeed, 0.0, 0.0));
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_W, true)) {
			view = glm::translate(view, glm::vec3(0.0, cameraSpeed, 0.0));
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_S, true)) {
			view = glm::translate(view, glm::vec3(0.0, -cameraSpeed, 0.0));
		}

		//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//update();
		//if (ImGui::IsMouseClicked(GLFW_MOUSE_BUTTON_1, false)) {
		//	//cout << "FIRST CLICK" << endl;
		//	prevPos = ImGui::GetMousePos();
		//}

		//if (ImGui::IsMouseClicked(GLFW_MOUSE_BUTTON_1, true)) {
		//	currentPos = ImGui::GetMousePos();
		//	//cout << "CONTINUE CLICK" << endl;
		//	//drag = true;
		//}
		//
		////if(drag){
		//	cout << "X diff: " << prevPos.x - currentPos.x << " Y diff: " << prevPos.y - currentPos.y << endl;
		//	currentPos = ImVec2(0, 0);
		//	//drag = false;
		////}
		


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Draw Menu
		menuDraw();
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		draw(view);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
