#pragma once

#include <barrier>
#include <vector>

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

        std::vector<std::unique_ptr<Factory>> steel_factories_;

        std::barrier<> sim_barrier_;
        std::atomic<bool> is_running_ ;

        void AdvanceTick();

        void CreateCollectors(Resource resource, int count, int units_per_tick);
        void CreateSteelFactories(int count);

        void PrintResources() const;

        Silo& SiloFor(Resource resource);
        const Silo& SiloFor(Resource resource) const;

    public:
        RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_steel_factories);

        void Run(int num_ticks);
        void Stop();
        void Join();
};