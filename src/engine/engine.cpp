#include <engine/engine.hpp>

#include <iostream>
#include <thread>
#include <chrono>


RobotBuilderEngine::RobotBuilderEngine()
{
    iron_silo_ = new Silo();
}

RobotBuilderEngine::~RobotBuilderEngine()
{
    delete iron_silo_;
}

void RobotBuilderEngine::Loop()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "\rIron: " << iron_silo_->GetStoredUnits() << std::flush;
}
