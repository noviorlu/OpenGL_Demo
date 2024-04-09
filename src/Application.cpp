#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new Application();
	}
	return mInstance;
}
Application::Application() { }

Application::~Application() {
	delete mInstance;
}

bool Application::initCanvas(const int& width, const int& height) {
	mWidth = width;
	mHeight = height;

	/* [GLFW] 1. initalize GLFW env */
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// core mode (not-immediate rendering)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* [GLFW] 2. create Window obj */
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL_Demo", NULL, NULL);
	if (!mWindow) {
		std::cout << "Failed to Create glfwWindow" << std::endl;
		glfwTerminate();
		return false;
	}
	// set current window to be Canvas
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1); // Enable vsync

	/* [Glew] Load all OpenGL function mapping */
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return false;
	}

	/* [GLFW] 2.1 register Callback */
	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);
	glfwSetKeyCallback(mWindow, keyEventCallBack);

	glfwSetWindowUserPointer(mWindow, this);

	return true;
}

bool Application::initGui()
{
	/* [ImGUI] */
// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)(*io);
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
	return false;
}

bool Application::checkCanvas() {
	if (glfwWindowShouldClose(mWindow)) return false;
	glfwPollEvents();

	return true;
}

void Application::swapCanvasBuffer()
{
	/* [GLFW] 3.2 DoubleBuffer swap */
	glfwSwapBuffers(mWindow);
}

void Application::destoryCanvas() {
	/* [GLFW] 4. Cleanup */
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void Application::destoryGui()
{
	// [ImGui] Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self != nullptr) self->mResizeCallback(width, height);
}

void Application::keyEventCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self != nullptr) self->mKeyEventCallback(key, scancode, action, mods);
}