#include "Layer.h"

#include "Application.h"

namespace Core
{

    void Layer::QueueTrasition(std::unique_ptr<Layer> toLayer)
    {
        auto& layerStack = Application::Get().m_LayerStack;

        for (auto& layer : layerStack)
        {
            if (layer.get() == this)
            {
                layer = std::move(toLayer); // Bad idea you -_- .... but ... this is a jam
                return;
            }
        }
    }

}