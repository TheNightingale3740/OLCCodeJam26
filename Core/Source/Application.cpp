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
        while (m_Running)
        {
            for (auto& layer: m_LayerStack)
            {
                layer->OnUpdate(5);
            }

            for (auto& layer: m_LayerStack)
            {
                layer->OnRender();
            }

            m_Running = false;
        }
        
        Shutdown();
    }

    void Application::Init()
    {
        std::println("Initializing stuff ...");
    }

    void Application::Shutdown()
    {
        std::println("Cleaning stuff up ...");

        for (auto& layer: m_LayerStack)
        {
            layer->OnDetach();
        }
    }

}