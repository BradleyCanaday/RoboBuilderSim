#include <engine/engine.hpp>
#include <iostream>

int main()
{
    RobotBuilderEngine engine(1,1,1,1,1);

    engine.Run(10);
    engine.Stop();
    engine.Join();

    std::cout << '\n';

    return 0;
}