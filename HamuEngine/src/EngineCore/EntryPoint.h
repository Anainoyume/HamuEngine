#pragma once

#include "Application.h"

#ifdef HAMU_PLATFORM_WINDOWS

extern HamuEngine::Application* HamuEngine::CreateApplication();

int main(int argc, char** argv) {

    auto app = HamuEngine::CreateApplication();
    app->Run();
    delete app;

}

#endif