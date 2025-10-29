#include "EngineCore/Application.h"
#include <HamuEngine.h>

class SandBox : public HamuEngine::Application
{
public:
    SandBox() {}

    ~SandBox() {}
};

HamuEngine::Application* HamuEngine::CreateApplication() {
    return new SandBox();
}