#pragma once
#include "glpch.h"
#include <GLCore.h>
#include <GLCoreUtils.h>

using namespace GLCore;

class TestMenu : public GLCore::Layer
{
public:
	TestMenu(Application* app);
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	
	void Preset(std::string testName);
	
	template<typename T>
	void RegisterTest(const std::string& name) {
		std::cout << "[TestMenu] Registering test: " << name << std::endl;
		m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
	}
private:
	Application* m_Application;
	
	Layer* m_currentLayer;
	std::vector<std::pair<std::string, std::function<Layer* ()>>> m_Tests;
};