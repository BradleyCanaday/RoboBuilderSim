#pragma once

#include <barrier>
#include <vector>

#include <factories/factory.hpp>
#include <resource_collectors/resource_collector.hpp>
#include <ingredients/silo.hpp>

class RobotBuilderEngine
{
    private:
        Silo iron_silo_;
        Silo coal_silo_;
        Silo steel_silo_;

        std::vector<std::unique_ptr<ResourceCollector>> iron_mines_;
        std::vector<std::unique_ptr<ResourceCollector>> coal_mines_;

        std::vector<std::unique_ptr<Factory>> steel_factories_;

        std::barrier<> sim_barrier_;
        std::atomic<bool> is_running_ ;

    public:
        RobotBuilderEngine(int num_iron_mines, int num_coal_mines, int num_steel_factories);

        void Run(int num_ticks);
        void Stop();
        void Join();
};