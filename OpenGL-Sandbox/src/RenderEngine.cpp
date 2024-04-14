#include "GLCore.h"
#include "TestMenu/TestMenu.h"
#include "ClearColorTest/ClearColorTest.h"
#include "TextureTest/Texture2DTest.h"
#include "BatchRenderTest/BatchRenderTest.h"

using namespace GLCore;

class RenderEngine : public Application
{
public:
	RenderEngine()
		: Application("RenderEngine")
	{
		TestMenu* testMenu = new TestMenu(this);
		testMenu->RegisterTest<ClearColorTest>("Test ColorQuad");
		testMenu->RegisterTest<Texture2DTest>("Test Texture2D");
		testMenu->RegisterTest<BatchRenderTest>("Test BatchRender");
		PushLayer(testMenu);

		testMenu->Preset("Test BatchRender");
	}
};

int main()
{
	RenderEngine* app = new RenderEngine();
	app->Run();
	delete app;
}