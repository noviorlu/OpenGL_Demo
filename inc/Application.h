#pragma once

#include <iostream>

#define app (*Application::getInstance())
struct GLFWwindow;

using ResizeCallback = void(*)(int width, int height);
using KeyEventCallback = void(*)(int key, int scancode, int action, int mods);

class Application {
public:
	~Application();
	static Application* getInstance();


	bool init(const int& width = 800, const int& height = 600);
	bool update();
	void destory();


	uint32_t getWidth()const { return mWidth; }
	uint32_t getHeight()const { return mHeight; }

	void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }
	void setKeyEventCallback(KeyEventCallback callback) { mKeyEventCallback = callback; }

private:
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void keyEventCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	// Instance, monoSignleton
	static Application* mInstance;

	Application();
	uint32_t mWidth{ 0 };
	uint32_t mHeight{ 0 };
	GLFWwindow* mWindow{ nullptr };

	ResizeCallback mResizeCallback{ nullptr };
	KeyEventCallback mKeyEventCallback{ nullptr };
};