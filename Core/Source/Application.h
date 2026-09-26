#pragma once

#include "Layer.h"

#include <concepts>
#include <memory>
#include <vector>

namespace Core
{

    class Application
    {
    public:
        Application();
        ~Application();

        template <typename TLayer>
        requires std::derived_from<TLayer, Layer>
        void PushLayer()
        {
            std::unique_ptr<TLayer> layer = std::make_unique<TLayer>();
            layer->OnAttach();
            
            m_LayerStack.push_back(std::move(layer));
        }

        void Run();

        void Init();
        void Shutdown();
    private:
        std::vector<std::unique_ptr<Layer>> m_LayerStack;
        
        bool m_Running = true;
    };

    extern std::unique_ptr<Application> CreateApplication();

}