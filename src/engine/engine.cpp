#include <engine/engine.hpp>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <thread>


#include <nlohmann/json.hpp>

RobotBuilderEngine::RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_copper_mines, 
                           int num_silicon_mines, int num_rubber_farms, int num_steel_factories, 
                           int num_wire_factories, int num_wheel_factories, int num_microcontroller_factories,
                           int num_motor_factories, int num_chassis_factories, int num_computer_factories, 
                           int num_robot_factories)
: sim_barrier_(num_iron_mines+num_coal_mines+num_copper_mines+ 
               num_silicon_mines+num_rubber_farms+num_steel_factories+ 
               num_wire_factories+num_wheel_factories+num_microcontroller_factories+
               num_motor_factories+num_chassis_factories+num_computer_factories+ 
               num_robot_factories+1),
  is_running_(true)
{
    LoadResources("data/resources");
    LoadBlueprints("data/blueprints");

    CreateCollectors("iron", num_iron_mines, resource_registry_["iron"].output_per_tick);
    CreateCollectors("coal", num_coal_mines, resource_registry_["coal"].output_per_tick);
    CreateCollectors("copper", num_copper_mines, resource_registry_["copper"].output_per_tick);
    CreateCollectors("silicon", num_silicon_mines, resource_registry_["silicon"].output_per_tick);
    CreateCollectors("rubber", num_rubber_farms, resource_registry_["rubber"].output_per_tick);

    CreateFactories("Steel", num_steel_factories);
    CreateFactories("Copper Wire", num_wire_factories);
    CreateFactories("Wheel", num_wheel_factories);
    CreateFactories("Microcontroller", num_microcontroller_factories);

    CreateFactories("Motor", num_motor_factories);
    CreateFactories("Chassis", num_chassis_factories);
    CreateFactories("Computer", num_computer_factories);

    CreateFactories("Robot", num_robot_factories);
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
    for (auto& collector : collectors_) {
        collector->Join();
    }

    for (auto& factory : factories_) {
        factory->Join();
    }
}

void RobotBuilderEngine::AdvanceTick()
{
    sim_barrier_.arrive_and_wait(); //T0 generation
    sim_barrier_.arrive_and_wait(); //T1 generation
    sim_barrier_.arrive_and_wait(); //T2 generation
    sim_barrier_.arrive_and_wait(); //T3 generation
    sim_barrier_.arrive_and_wait(); //T4 generation
}

void RobotBuilderEngine::LoadResources(const std::string& directory_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (entry.path().extension() == ".json")
        {
            std::ifstream file(entry.path());
            nlohmann::json j;
            file >> j;

            Resource resource;
            resource.id = j["id"];
            resource.name = j["name"];
            resource.output_per_tick = j["output_per_tick"];
            resource.tier= j["tier"];

            resource_registry_[resource.id] = resource;
            silos_[resource.id] = std::make_unique<Silo>(resource.id);
        }
    }
    std::cout << "Loaded " << resource_registry_.size() << " resources.\n";
}

void RobotBuilderEngine::LoadBlueprints(const std::string& directory_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        if (entry.path().extension() == ".json")
        {
            std::ifstream file(entry.path());
            nlohmann::json j;
            file >> j;

            Blueprint bp;
            bp.name = j["name"];
            bp.output_type = j["output_type"];

            for (auto& [input_id, amount] : j["allowed_input_ports"].items()) 
            {
                bp.allowed_input_ports[input_id] = amount.get<int>();
            }

            blueprints_[bp.name] = bp;
        }
    }
    std::cout << "Loaded " << blueprints_.size() << " blueprints.\n";
}

void RobotBuilderEngine::CreateCollectors(const std::string& resource, int count, int units_per_tick)
{
    auto& silo = SiloFor(resource);

    for (int i = 0; i < count; ++i)
    {
        // Just push directly into the single collectors_ vector
        collectors_.push_back(std::make_unique<ResourceCollector>(
            silo,
            sim_barrier_,
            units_per_tick,
            is_running_,
            5));

        collectors_.back()->Start();
    }
}

void RobotBuilderEngine::CreateFactories(const std::string& blueprint_name, int count)
{
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
            resource_registry_.at(bp.output_type).output_per_tick, // <-- Grab the amount from the registry!
            is_running_,
            5,
            resource_registry_.at(bp.output_type).tier));

        factories_.back()->Start();
    }
}

void RobotBuilderEngine::PrintResources() const
{
    std::cout << '\r';

    std::vector<std::string> keys;
    for (const auto& [id, silo_ptr] : silos_)
    {
        keys.push_back(id);
    }
    std::sort(keys.begin(), keys.end());

    for (const auto& id : keys)
    {
        const std::string& nice_name = resource_registry_.at(id).name;
        std::cout << nice_name << ": " << silos_.at(id)->GetStoredUnits() << " | ";
    }
    
    std::cout << "          " << std::flush; 
}

Silo& RobotBuilderEngine::SiloFor(const std::string& resource_id)
{
    return *silos_.at(resource_id);
}

const Silo& RobotBuilderEngine::SiloFor(const std::string& resource_id) const
{
    return *silos_.at(resource_id);
}
