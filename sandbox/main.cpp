#include "Lesson4_Behavior.h"

int main() {

    hamu::Application app("Lesson 4 - Logic Behavior");
    app.AddBehavior<TestBehavior>(app);
    app.Run();

    return 0;
}