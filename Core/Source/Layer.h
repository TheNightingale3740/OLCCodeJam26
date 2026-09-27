#pragma once

#include <memory>

namespace Core
{

    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() {}

        virtual void OnAttach()         {}
        virtual void OnDetach()         {}
        virtual void OnUpdate(float ts) {}
        virtual void OnUIRender()       {}
        virtual void OnRender()         {}

        template <std::derived_from<Layer> T, typename ...Args>
        void TransitionTo(Args&&... args)
        {
            QueueTrasition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
        }

    private:
        void QueueTrasition(std::unique_ptr<Layer> toLayer);
    };

}