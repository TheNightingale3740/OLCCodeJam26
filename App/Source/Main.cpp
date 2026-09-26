#include <Application.h>

#include <memory>
#include <print>

//#include <raylib.h>

// #ifdef PLATFORM_WEB
//     #include <emcripten/emscripten.h>
// #endif

class ExampleLayer : public Core::Layer
{
public:
    ExampleLayer()
    {

    }

    ~ExampleLayer()
    {

    }

    void OnRender() override
    {
        std::println("I am rendering stuff ...");
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