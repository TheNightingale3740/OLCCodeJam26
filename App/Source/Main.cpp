#include "Layer.h"
#include <Application.h>

#include <imgui.h>
#include <raylib.h>

#include "MainMenu.h"

class DebugOverlay : public Core::Layer
{
public:
    DebugOverlay() = default;
    ~DebugOverlay() {}

    void OnUIRender() override
    {
        ImGui::Begin("Performance Stats");
        ImGui::Text("Last Render: %.3f", GetFrameTime());
        ImGui::End();
    }
};

std::unique_ptr<Core::Application> Core::CreateApplication()
{
    std::unique_ptr<Core::Application> app = std::make_unique<Core::Application>();
    return std::move(app);
}

int main()
{
    std::unique_ptr<Core::Application> app = Core::CreateApplication();
    app->PushLayer<MainMenu>();
    app->PushLayer<DebugOverlay>();
    app->Run();
}