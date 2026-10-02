#include <raylib.h>
#include <rlImGui.h>

#include "Application.h"

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

    Application& Application::Get()
    {
        return *s_Application;
    }

    void Application::MainLoop()
    {
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto& layer: m_LayerStack)
            layer->OnUpdate(GetFrameTime());

        
        for (auto& layer: m_LayerStack)
            layer->OnRender();
        
        rlImGuiBegin();
        
        for (auto& layer: m_LayerStack)
            layer->OnUIRender();
        
        rlImGuiEnd();

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

        ChangeDirectory(GetApplicationDirectory());

        #ifndef PLATFORM_WEB
        SetWindowState(FLAG_FULLSCREEN_MODE);
        #endif
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