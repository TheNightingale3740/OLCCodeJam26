#include <raylib.h>
#include <rlImGui.h>

#include "Application.h"

#include <print>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

namespace Core
{

    static Application *s_Application = nullptr;

    static void StartMainLoop()
    {
        if (s_Application)
        {
            s_Application->MainLoop();
        }
    }

    void Application::MainLoop()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello, world!", 500, 500, 16, WHITE);

        for (auto& layer: m_LayerStack)
            layer->OnUpdate(GetFrameTime());

        rlImGuiBegin();
        for (auto& layer: m_LayerStack)
            layer->OnUIRender();
        rlImGuiEnd();

        for (auto& layer: m_LayerStack)
            layer->OnRender();

        EndDrawing();
    }

    Application::Application()
    {
        s_Application = this;
    }

    Application::~Application()
    {
        s_Application = nullptr;
    }

    void Application::Run()
    {
        Init();

        #ifdef PLATFORM_WEB
        emscripten_set_main_loop(StartMainLoop, 0, 1);
        #else
        while (!WindowShouldClose())
        {
            MainLoop();
        }
        #endif
        
        Shutdown();
    }

    void Application::Init()
    {
        InitWindow(1280, 720, "OLC Code Jam 2026");

        SetTargetFPS(60);

        rlImGuiSetup(true);
    }

    void Application::Shutdown()
    {
        for (auto& layer: m_LayerStack)
        {
            layer->OnDetach();
        }

        rlImGuiShutdown();
        CloseWindow();
    }

}