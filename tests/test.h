#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include <GL/glew.h>
#include <imgui/imgui.h>
#include "GLError.h"

namespace test {
	class Test {
	public:
		// creation and descruction of VAO stuff should all be in the
		// test's constructor and desctuctor, replace the onInit and Deinit
		Test(){}
		virtual ~Test(){}
		
		virtual void OnUpdate(float deltaTime){}
		virtual void OnRender(){}
		virtual void OnImGuiRender(){}
	};

	class TestMenu : public Test {
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name) {
			std::cout << "[TestMenu] Registering test: " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		// functions that return a Test*
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}