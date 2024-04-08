#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

bool Application::init(const int& width, const int& height) {
	mWidth = width;
	mHeight = height;

	/* [GLFW] 1. initalize GLFW env */
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	/* [Glad] Load all OpenGL function mapping */
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

bool Application::update() {
	if (glfwWindowShouldClose(mWindow)) return false;
	/*
	 * [GLFW] 3.1 pull and cast window message
	 * ex.mouse, keyboard. if has msg, deal and clean queue
	 */
	glfwPollEvents();

	/* [GLFW] 3.2 DoubleBuffer swap */
	glfwSwapBuffers(mWindow);
	return true;
}

void Application::destory() {
	/* [GLFW] 4. Cleanup */
	glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self != nullptr) self->mResizeCallback(width, height);
}

void Application::keyEventCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self != nullptr) self->mKeyEventCallback(key, scancode, action, mods);
}