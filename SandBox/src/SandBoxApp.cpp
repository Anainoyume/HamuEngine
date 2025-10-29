#include <HamuEngine.h>

class SandBox : public HamuEngine::Application
{
public:
    SandBox() {}

    ~SandBox() {}
};

int main() {

    auto app = new SandBox();
    app->Run();
    delete app;
}