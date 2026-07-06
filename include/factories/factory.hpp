#pragma once

#include <ingredients/silo.hpp>

#include <atomic>
#include <barrier>
#include <thread>
#include <vector>

class Factory
{
    private:
        std::vector<Silo*> input_silos_;
        std::vector<bool> has_input_;
        std::vector<int> input_volumes_;

        Silo& output_silo_;
        int production_rate_;

        std::barrier<>& sim_barrier_;
        std::thread collector_thread_;

        std::atomic<bool>& is_running_;

    public:
        Factory(std::vector<Silo*> input_silos, std::vector<int> input_volumes, Silo& output_silo, 
                std::barrier<>& sim_barrier, int production_rate, std::atomic<bool>& is_running);

        void Start();
        void Run();
        void Join();        
};