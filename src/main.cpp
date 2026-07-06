#include <engine/engine.hpp>
#include <iostream>

int main()
{
    RobotBuilderEngine engine(5,5,5,
                              5,5,7,
                              5,4,12,
                              4,1,1,
                              1);

    engine.Run(10);
    engine.Stop();
    engine.Join();

    std::cout << '\n';

    return 0;
}