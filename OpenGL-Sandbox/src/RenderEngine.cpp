#include "GLCore.h"
#include "TestMenu/TestMenu.h"
#include "ClearColorTest/ClearColorTest.h"
#include "TextureTest/Texture2DTest.h"
#include "BatchRenderTest/BatchRenderTest.h"
#include "CubeTest/CubeTest.h"

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
		testMenu->RegisterTest<CubeTest>("Test cube3D");
		PushLayer(testMenu);

		testMenu->Preset("Test cube3D");
	}
};

int main()
{
	RenderEngine* app = new RenderEngine();
	app->Run();
	delete app;
}