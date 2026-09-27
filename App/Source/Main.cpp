#include "Layer.h"
#include <Application.h>

#include <imgui.h>
#include <raylib.h>

// cd ~/Documents/Dev/emsdk && source ./emsdk_env.sh && cd ~/Documents/Dev/OLCCodeJam26
// make config=web CC=emcc CXX=em++ AR=emar
// emrun bin/Web/App.html

class TransitionLayer : public Core::Layer
{
public:
    TransitionLayer()
    {

    }

    ~TransitionLayer()
    {

    }

    void OnUIRender() override
    {
        ImGui::ShowDemoWindow();
    }
};

class ExampleLayer : public Core::Layer
{
public:
    ExampleLayer()
    {

    }

    ~ExampleLayer()
    {

    }

    void OnUpdate(float ts) override
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            TransitionTo<TransitionLayer>();
        }
    }

    void OnUIRender() override
    {
        ImGui::Begin("Hello, Window!");
        ImGui::Text("Hello, Text");
        ImGui::End();
    }
};

std::unique_ptr<Core::Application> Core::CreateApplication()
{
    std::unique_ptr<Core::Application> app = std::make_unique<Core::Application>();
    app->PushLayer<ExampleLayer>();
    return std::move(app);
}

int main()
{
    std::unique_ptr<Core::Application> app = Core::CreateApplication();
    app->Run();
}