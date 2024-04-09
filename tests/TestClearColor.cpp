#include "TestClearColor.h"

namespace test {
    TestClearColor::TestClearColor()
     : m_ClearColor { 0.2f, 0.3f, 0.3f, 1.0f } { }
    TestClearColor::~TestClearColor() { 
        GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    }

    void TestClearColor::OnRender()
    {
        GL_CALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
