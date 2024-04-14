#include "GLCore.h"
#include "TestMenu/TestMenu.h"
#include "ClearColorTest/ClearColorTest.h"

using namespace GLCore;

class RenderEngine : public Application
{
public:
	RenderEngine()
		: Application("RenderEngine")
	{
		TestMenu* testMenu = new TestMenu(this);
		testMenu->RegisterTest<ClearColorTest>("Test ColorQuad");
		PushLayer(testMenu);
	}
};

int main()
{
	RenderEngine* app = new RenderEngine();
	app->Run();
	delete app;
}