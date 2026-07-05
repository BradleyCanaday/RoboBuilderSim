#include <engine/engine.hpp>
#include <iostream>

int main()
{
    RobotBuilderEngine* engine = new RobotBuilderEngine();

    int i=0;
    while(i<10)
    {
        engine->Loop();
        i++;
    }

    std::cout << '\n';

    return 0;
}