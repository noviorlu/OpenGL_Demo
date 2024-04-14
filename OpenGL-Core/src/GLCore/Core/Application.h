#pragma once

#include <queue>

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "../Events/Event.h"
#include "../Events/ApplicationEvent.h"

#include "Timestep.h"

#include "../ImGui/ImGuiLayer.h"

namespace GLCore {
	enum e_ApplicationLayerInstr
	{
		PUSH_LAYER,
		POP_LAYER,
		POP_LAYER_CLEAN,
		PUSH_OVERLAY,
		POP_OVERLAY
	};
	
	class Application
	{
	public:
		Application(const std::string& name = "OpenGL Sandbox", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);
		void RuntimeLayerInstr(e_ApplicationLayerInstr instr, Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	
		

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		void PostUpdate();
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		
		
		std::queue<std::pair<e_ApplicationLayerInstr, Layer*>> m_LayerInstrCache;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

}