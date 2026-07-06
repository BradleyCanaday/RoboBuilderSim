#include <engine/engine.hpp>

#include <iostream>
#include <thread>
#include <chrono>


RobotBuilderEngine::RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_steel_factories)
: sim_barrier_(num_iron_mines+num_coal_mines+num_steel_factories+1),
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

    for(int i=0; i<num_coal_mines; i++)
    {
        auto coal_mine = std::make_unique<ResourceCollector>(
        coal_silo_,
        sim_barrier_,
        5,
        is_running_
        );

        coal_mine->Start();
        coal_mines_.push_back(std::move(coal_mine));
    }

    std::vector<Silo*> steel_input_silos;
    steel_input_silos.push_back(&iron_silo_);
    steel_input_silos.push_back(&coal_silo_);

    std::vector<int> input_volumes = {10,5};

    for(int i=0; i<num_steel_factories; i++)
    {
        auto steel_factory = std::make_unique<Factory>(
        steel_input_silos,
        input_volumes,
        steel_silo_,
        sim_barrier_,
        1,
        is_running_
        );

        steel_factory->Start();
        steel_factories_.push_back(std::move(steel_factory));
    }
}

void RobotBuilderEngine::Run(int ticks)
{
    for (int i = 0; i < ticks; i++)
    {
        sim_barrier_.arrive_and_wait();
        sim_barrier_.arrive_and_wait();
        sim_barrier_.arrive_and_wait();
        std::cout << "\rIron: "
                  << iron_silo_.GetStoredUnits()
                  << " Coal: "
                  << coal_silo_.GetStoredUnits()
                  << " Steel: "
                  << steel_silo_.GetStoredUnits()
                  << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
}

void RobotBuilderEngine::Stop()
{
    is_running_ = false;
    sim_barrier_.arrive_and_wait();
    sim_barrier_.arrive_and_wait();
    sim_barrier_.arrive_and_wait();
}

void RobotBuilderEngine::Join()
{
    for (auto& mine : iron_mines_)
        mine->Join();

    for (auto& mine : coal_mines_)
        mine->Join();

    for (auto& factory : steel_factories_)
        factory->Join();
}
