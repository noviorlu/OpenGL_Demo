#include "glpch.h"
#include "Application.h"

#include "Log.h"

#include "Input.h"

#include <glfw/glfw3.h>

namespace GLCore {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, uint32_t width, uint32_t height)
	{
		if (!s_Instance)
		{
			// Initialize core
			Log::Init();
		}

		GLCORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_LayerStack.PopOverlay(layer);
	}

	void Application::RuntimeLayerInstr(e_ApplicationLayerInstr instr, Layer* layer)
	{
		m_LayerInstrCache.push({ instr, layer });
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			/* Post Process */
			PostUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PostUpdate() {
		while (!m_LayerInstrCache.empty()) {
			auto instr = m_LayerInstrCache.front();
			switch (instr.first) {
			case PUSH_LAYER:
				m_LayerStack.PushLayer(instr.second);
				break;
			case POP_LAYER:
				m_LayerStack.PopLayer(instr.second);
				break;
			case POP_LAYER_CLEAN:
				m_LayerStack.PopLayer(instr.second);
				delete instr.second;
				break;
			case PUSH_OVERLAY:
				m_LayerStack.PushOverlay(instr.second);
				break;
			case POP_OVERLAY:
				m_LayerStack.PopOverlay(instr.second);
				break;
			}
			m_LayerInstrCache.pop();
		}
	}
}