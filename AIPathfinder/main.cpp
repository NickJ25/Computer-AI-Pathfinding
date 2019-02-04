
#include "vendors/imgui.h"
#include "vendors/imgui_impl_glfw.h"
#include "vendors/imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/glew.h>  
#include <GLFW/glfw3.h> 

bool show_about_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void update() {

}

void draw() {

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
	const char* items[] = { "A*", "BBBB", "CCCC" };
	static int item_current = 0;
	ImGui::Combo("Algorithm", &item_current, items, IM_ARRAYSIZE(items));

	// Start Node
	static char str0[2] = "A";
	ImGui::PushItemWidth(100);
	ImGui::InputText("Start Node", str0, IM_ARRAYSIZE(str0));

	// End Node
	ImGui::PushItemWidth(100);
	static char str1[2] = "B";
	ImGui::InputText("End Node", str1, IM_ARRAYSIZE(str1));

	// Find Button
	if (ImGui::Button("Find")) {

	}
	ImGui::SameLine();
	// About Button
	if (ImGui::Button("About")) {
		if(!show_about_window)
			show_about_window = true;
		else
			show_about_window = false;
	}
			
	ImGui::Text("avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	// 3. Show another simple window.
	if (show_about_window)
	{
		//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(240, 140), ImGuiCond_Once);
		ImGui::Begin("About", &show_about_window, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("A simple pathfinding showcase \ncreated by Niclas Jonsson and \nDavid Walbancke. \n\nThis project uses:\n-dear imgui (v1.67)\n-glew \n-glfw");
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

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		update();
		draw();

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
