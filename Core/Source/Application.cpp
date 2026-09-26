#include <raylib.h>

#include "Application.h"

#include <print>
namespace Core
{

    Application::Application()
    {
        std::println("Created Application");
    }

    Application::~Application()
    {
        std::println("Destroyed Application!");
    }

    void Application::Run()
    {
        Init();

        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("Hello, world!", 500, 500, 16, WHITE);

            for (auto& layer: m_LayerStack)
            {
                layer->OnUpdate(GetFrameTime());
            }

            for (auto& layer: m_LayerStack)
            {
                layer->OnUIRender();
            }

            for (auto& layer: m_LayerStack)
            {
                layer->OnRender();
            }

            EndDrawing();
        }
        
        Shutdown();
    }

    void Application::Init()
    {
        std::println("Initializing stuff ...");

        InitWindow(1280, 720, "OLC Code Jam 2026");

        SetTargetFPS(60);
    }

    void Application::Shutdown()
    {
        std::println("Cleaning stuff up ...");

        for (auto& layer: m_LayerStack)
        {
            layer->OnDetach();
        }

        CloseWindow();
    }

}