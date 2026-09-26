#pragma once

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
    };

}