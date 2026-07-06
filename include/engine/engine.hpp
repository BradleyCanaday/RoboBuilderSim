#pragma once

#include <barrier>
#include <map>
#include <string>
#include <vector>

#include <blueprints/blueprint.hpp>
#include <factories/factory.hpp>
#include <resource_collectors/resource_collector.hpp>
#include <ingredients/silo.hpp>
#include <ingredients/resource_types.hpp>

class RobotBuilderEngine
{
    private:
        std::array<Silo, ToIndex(Resource::Count)> silos_;

        std::array<
            std::vector<std::unique_ptr<ResourceCollector>>,
            ToIndex(Resource::Count)
        > collectors_;

        std::vector<std::unique_ptr<Factory>> factories_;

        std::map<std::string, Blueprint> blueprints_;

        std::barrier<> sim_barrier_;
        std::atomic<bool> is_running_ ;

        void AdvanceTick();

        void GenerateBlueprints();
        void CreateCollectors(Resource resource, int count, int units_per_tick);
        void CreateFactories(const std::string& blueprint_name, int count);

        void PrintResources() const;

        Silo& SiloFor(Resource resource);
        const Silo& SiloFor(Resource resource) const;

    public:
        RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_copper_mines, int num_steel_factories, int num_wire_factories);

        void Run(int num_ticks);
        void Stop();
        void Join();
};