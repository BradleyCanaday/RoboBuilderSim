#include <engine/engine.hpp>

#include <iostream>
#include <thread>
#include <chrono>

RobotBuilderEngine::RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_steel_factories)
: sim_barrier_(num_iron_mines+num_coal_mines+num_steel_factories+1),
  is_running_(true)
{
    CreateCollectors(
        Resource::Iron,
        num_iron_mines,
        GetResourceInfo(Resource::Iron).collector_output_per_tick);

    CreateCollectors(
        Resource::Coal,
        num_coal_mines,
        GetResourceInfo(Resource::Coal).collector_output_per_tick);

    CreateSteelFactories(num_steel_factories);
}

void RobotBuilderEngine::Run(int ticks)
{
    for (int i = 0; i < ticks; i++)
    {
        AdvanceTick();
        PrintResources();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
}

void RobotBuilderEngine::Stop()
{
    is_running_ = false;
    AdvanceTick();
}

void RobotBuilderEngine::Join()
{
    for (auto& collector_list : collectors_)
    {
        for (auto& collector : collector_list)
            collector->Join();
    }

    for (auto& factory : steel_factories_)
        factory->Join();
}

void RobotBuilderEngine::AdvanceTick()
{
    sim_barrier_.arrive_and_wait(); //T0 generation
    sim_barrier_.arrive_and_wait(); //T1 generation
    sim_barrier_.arrive_and_wait(); //T2 generation
}

void RobotBuilderEngine::CreateCollectors(
    Resource resource,
    int count,
    int units_per_tick)
{
    auto& collectors = collectors_[ToIndex(resource)];
    auto& silo = SiloFor(resource);

    for (int i = 0; i < count; ++i)
    {
        collectors.push_back(std::make_unique<ResourceCollector>(
            silo,
            sim_barrier_,
            units_per_tick,
            is_running_));

        collectors.back()->Start();
    }
}

void RobotBuilderEngine::CreateSteelFactories(int count)
{
    const std::vector<Silo*> inputs{
        &SiloFor(Resource::Iron),
        &SiloFor(Resource::Coal)
    };

    const std::vector<int> volumes{
        GetResourceInfo(Resource::Iron).collector_output_per_tick,
        GetResourceInfo(Resource::Coal).collector_output_per_tick
    };

    for (int i = 0; i < count; ++i)
    {
        steel_factories_.push_back(std::make_unique<Factory>(
            inputs,
            volumes,
            SiloFor(Resource::Steel),
            sim_barrier_,
            1,
            is_running_));

        steel_factories_.back()->Start();
    }
}

void RobotBuilderEngine::PrintResources() const
{
    std::cout
        << '\r'
        << GetResourceInfo(Resource::Iron).name << ": "
        << SiloFor(Resource::Iron).GetStoredUnits()
        << " "
        << GetResourceInfo(Resource::Coal).name << ": "
        << SiloFor(Resource::Coal).GetStoredUnits()
        << " "
        << GetResourceInfo(Resource::Steel).name << ": "
        << SiloFor(Resource::Steel).GetStoredUnits()
        << std::flush;
}

Silo& RobotBuilderEngine::SiloFor(Resource resource)
{
    return silos_[ToIndex(resource)];
}

const Silo& RobotBuilderEngine::SiloFor(Resource resource) const
{
    return silos_[ToIndex(resource)];
}
