#pragma once

#include <ingredients/silo.hpp>

#include <atomic>
#include <barrier>
#include <thread>

class ResourceCollector
{
    private:
        Silo& output_silo_;
        int production_rate_;

        std::barrier<>& sim_barrier_;
        std::thread collector_thread_;

        std::atomic<bool>& is_running_;

    public:
        ResourceCollector(Silo& output_silo, std::barrier<>& sim_barrier, int production_rate, std::atomic<bool>& is_running);

        void Start();
        void Run();
        void Join();        
};