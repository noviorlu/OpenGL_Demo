#include "TestMenu.h"
#include "imgui.h"
using namespace GLCore;
using namespace GLCore::Utils;

TestMenu::TestMenu(Application* app)
	: m_Application(app), m_currentLayer(this)
{
	m_DebugName = "Test Menu";
}

void TestMenu::OnUpdate(Timestep ts) 
{
	if (m_currentLayer == this)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void TestMenu::OnImGuiRender()
{
	ImGui::Begin("Test Menu");
	if(m_currentLayer != this)
	{
		if (m_currentLayer != nullptr && ImGui::Button("<-"))
		{
			m_Application->RuntimeLayerInstr(POP_LAYER_CLEAN, m_currentLayer);
			m_currentLayer = this;
		}
	}
	else 
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_currentLayer = test.second();
				m_Application->RuntimeLayerInstr(PUSH_LAYER, m_currentLayer);
			}
		}
	}
	ImGui::End();
}