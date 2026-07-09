#pragma once

namespace hamu
{

    class Behavior
    {
    public:
        virtual ~Behavior() = default;

        virtual void Start() {}
        virtual void Update([[maybe_unused]] float dt) {}
        virtual void Destroy() {}

        virtual void OnDrawImGui() {}

        virtual void OnCursorPosChanged([[maybe_unused]] double xpos, [[maybe_unused]] double ypos) {}
        virtual void OnFramebufferResized([[maybe_unused]] int width, [[maybe_unused]] int height) {}
    };

} // namespace hamu