#pragma once

#include <barrier>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <atomic>

#include <blueprints/blueprint.hpp>
#include <factories/factory.hpp>
#include <resource_collectors/resource_collector.hpp>
#include <ingredients/silo.hpp>
#include <ingredients/resource_types.hpp>

class RobotBuilderEngine
{
    private:
        std::unordered_map<std::string, Resource> resource_registry_;
        std::map<std::string, Blueprint> blueprints_;

        std::unordered_map<std::string, std::unique_ptr<Silo>> silos_;

        std::vector<std::unique_ptr<ResourceCollector>> collectors_;
        std::vector<std::unique_ptr<Factory>> factories_;

        std::barrier<> sim_barrier_;
        std::atomic<bool> is_running_;

        void AdvanceTick();

        void LoadResources(const std::string& directory_path);
        void LoadBlueprints(const std::string& directory_path);

        void CreateCollectors(const std::string& resource_id, int count, int units_per_tick);
        void CreateFactories(const std::string& blueprint_name, int count);

        void PrintResources() const;

        Silo& SiloFor(const std::string& resource_id);
        const Silo& SiloFor(const std::string& resource_id) const;

    public:
        RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_copper_mines, 
                           int num_silicon_mines, int num_rubber_farms, int num_steel_factories, 
                           int num_wire_factories, int num_wheel_factories, int num_microcontroller_factories,
                           int num_motor_factories, int num_chassis_factories, int num_computer_factories, 
                           int num_robot_factories);

        void Run(int num_ticks);
        void Stop();
        void Join();
};