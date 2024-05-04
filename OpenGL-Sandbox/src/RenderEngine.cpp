#include "GLCore.h"
#include "TestMenu/TestMenu.h"
#include "ClearColorTest/ClearColorTest.h"
#include "TextureTest/Texture2DTest.h"
#include "BatchRenderTest/BatchRenderTest.h"
#include "CubeTest/CubeTest.h"
#include "ModelTest/ModelTest.h"
#include "MatTest/MatTest.h"

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
		testMenu->RegisterTest<ModelTest>("Test Model");
		testMenu->RegisterTest<MatTest>("Test Material");
		PushLayer(testMenu);

		testMenu->Preset("Test Material");
	}
};

int main()
{
	RenderEngine* app = new RenderEngine();
	app->Run();
	delete app;
}