#pragma once

#include <barrier>
#include <vector>

#include <resource_collectors/resource_collector.hpp>
#include <ingredients/silo.hpp>

class RobotBuilderEngine
{
    private:
        Silo iron_silo_;
        std::vector<std::unique_ptr<ResourceCollector>> iron_mines_;

        std::barrier<> sim_barrier_;
        std::atomic<bool> is_running_ ;

    public:
        RobotBuilderEngine(int num_iron_mines);

        void Run(int num_ticks);
        void Stop();
        void Join();
};