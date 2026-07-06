#include <engine/engine.hpp>

#include <iostream>
#include <thread>
#include <chrono>

RobotBuilderEngine::RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_copper_mines, int num_steel_factories, int num_wire_factories)
: sim_barrier_(num_iron_mines+num_coal_mines+num_copper_mines+num_steel_factories+num_wire_factories+1),
  is_running_(true)
{
    GenerateBlueprints();

    CreateCollectors(
        Resource::Iron,
        num_iron_mines,
        GetResourceInfo(Resource::Iron).output_per_tick);

    CreateCollectors(
        Resource::Coal,
        num_coal_mines,
        GetResourceInfo(Resource::Coal).output_per_tick);

    CreateCollectors(
        Resource::Copper,
        num_copper_mines,
        GetResourceInfo(Resource::Copper).output_per_tick);

    CreateFactories("Steel", num_steel_factories);
    CreateFactories("Wire", num_wire_factories);
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

    for (auto& factory : factories_)
    {
        factory->Join();
    }
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

void RobotBuilderEngine::CreateFactories(const std::string& blueprint_name, int count)
{
    // Safety check just in case you mistype a blueprint name later
    if (blueprints_.find(blueprint_name) == blueprints_.end()) {
        std::cerr << "Error: Blueprint '" << blueprint_name << "' not found!\n";
        return; 
    }

    const Blueprint& bp = blueprints_[blueprint_name];

    std::vector<Silo*> inputs;
    std::vector<int> volumes;

    for (const auto& [resource_type, amount_needed] : bp.allowed_input_ports)
    {
        inputs.push_back(&SiloFor(resource_type));
        volumes.push_back(amount_needed);
    }

    for (int i = 0; i < count; ++i)
    {
        factories_.push_back(std::make_unique<Factory>(
            inputs,
            volumes,
            SiloFor(bp.output_type),
            sim_barrier_,
            bp.output_amount,
            is_running_));

        factories_.back()->Start();
    }
}

void RobotBuilderEngine::GenerateBlueprints()
{
    Blueprint steel_blueprint;
    steel_blueprint.name = "Steel";
    steel_blueprint.allowed_input_ports[Resource::Iron] = 10; 
    steel_blueprint.allowed_input_ports[Resource::Coal] = 5; 
    steel_blueprint.output_type = Resource::Steel;
    steel_blueprint.output_amount = GetResourceInfo(Resource::Steel).output_per_tick;
    blueprints_[steel_blueprint.name] = steel_blueprint;

    Blueprint wire_blueprint;
    wire_blueprint.name = "Wire";
    wire_blueprint.allowed_input_ports[Resource::Copper] = 2; 
    wire_blueprint.output_type = Resource::Wire;
    wire_blueprint.output_amount = GetResourceInfo(Resource::Wire).output_per_tick;
    blueprints_[wire_blueprint.name] = wire_blueprint;
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
        << GetResourceInfo(Resource::Copper).name << ": "
        << SiloFor(Resource::Copper).GetStoredUnits()
        << " "
        << GetResourceInfo(Resource::Steel).name << ": "
        << SiloFor(Resource::Steel).GetStoredUnits()
        << " "
        << GetResourceInfo(Resource::Wire).name << ": "
        << SiloFor(Resource::Wire).GetStoredUnits()
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
