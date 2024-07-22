#include "GLCore.h"
#include "TestMenu/TestMenu.h"
#include "ClearColorTest/ClearColorTest.h"
#include "TextureTest/Texture2DTest.h"
#include "BatchRenderTest/BatchRenderTest.h"
#include "CubeTest/CubeTest.h"
#include "ModelTest/ModelTest.h"
#include "MatTest/MatTest.h"
#include "DepthTest/DepthTest.h"
#include "StencilTest/StencilTest.h"
#include "PostProcessTest/PostProcessTest.h"
#include "BlinnPhongTest/BlinnPhongTest.h"
#include "PPMTest/PPMTest.h"
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
		//testMenu->RegisterTest<BatchRenderTest>("Test BatchRender");
		//testMenu->RegisterTest<CubeTest>("Test cube3D");
		//testMenu->RegisterTest<ModelTest>("Test Model");
		//testMenu->RegisterTest<MatTest>("Test Material");
		testMenu->RegisterTest<DepthTest>("Test Depth");
		testMenu->RegisterTest<StencilTest>("Test Stencil");
		testMenu->RegisterTest<PostProcessTest>("Test PostProcess");
		testMenu->RegisterTest<BlinnPhongTest>("Test BlinnPhong");
		testMenu->RegisterTest<PPMTest>("Test PPM");
		PushLayer(testMenu);

		testMenu->Preset("Test PPM");
	}
};

int main()
{
	RenderEngine* app = new RenderEngine();
	app->Run();
	delete app;
}