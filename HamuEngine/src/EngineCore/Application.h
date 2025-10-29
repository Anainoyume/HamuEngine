#pragma once

#include "Core.h"

namespace HamuEngine {

    class HAMU_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

} // namespace HamuEngine
