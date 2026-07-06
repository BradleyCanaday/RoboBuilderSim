#include <engine/engine.hpp>

#include <iostream>
#include <thread>
#include <chrono>


RobotBuilderEngine::RobotBuilderEngine(int num_iron_mines)
: sim_barrier_(num_iron_mines+1),
  is_running_(true)
{
    for(int i=0; i<num_iron_mines; i++)
    {
        auto iron_mine = std::make_unique<ResourceCollector>(
        iron_silo_,
        sim_barrier_,
        10,
        is_running_
        );

        iron_mine->Start();
        iron_mines_.push_back(std::move(iron_mine));
    }
}

void RobotBuilderEngine::Run(int ticks)
{
    for (int i = 0; i < ticks; i++)
    {
        sim_barrier_.arrive_and_wait();
        sim_barrier_.arrive_and_wait();
        std::cout << "\rIron: "
                  << iron_silo_.GetStoredUnits()
                  << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
}

void RobotBuilderEngine::Stop()
{
    is_running_ = false;
    sim_barrier_.arrive_and_wait();
    sim_barrier_.arrive_and_wait();
}

void RobotBuilderEngine::Join()
{
    for (auto& mine : iron_mines_)
        mine->Join();
}
