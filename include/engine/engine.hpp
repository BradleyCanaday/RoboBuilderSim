#pragma once

#include <ingredients/silo.hpp>

class RobotBuilderEngine
{
    private:
        Silo* iron_silo_;

    public:
        RobotBuilderEngine();
        ~RobotBuilderEngine();

        void Loop();
};