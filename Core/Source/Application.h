#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "Layer.h"

namespace Core
{
    class Application
    {
        friend class Layer;
    public:
        Application();
        ~Application();

        static Application& Get();

        template <typename TLayer>
        requires std::derived_from<TLayer, Layer>
        void PushLayer()
        {
            std::unique_ptr<TLayer> layer = std::make_unique<TLayer>();
            layer->OnAttach();
            
            m_LayerStack.push_back(std::move(layer));
        }

        void Run();

        void MainLoop();

        void Init();
        void Shutdown();
    private:
        std::vector<std::unique_ptr<Layer>> m_LayerStack;
    };

    extern std::unique_ptr<Application> CreateApplication();

}