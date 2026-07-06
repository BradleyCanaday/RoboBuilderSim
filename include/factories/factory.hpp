#pragma once

#include <atomic>
#include <barrier>
#include <vector>

#include <engine/simulation_worker.hpp>
#include <ingredients/silo.hpp>

class Factory: public SimulationWorker
{
    private:
        void Step(int current_phase) override;

        std::vector<Silo*> input_silos_;
        std::vector<int> input_volumes_;
        std::vector<bool> has_input_;

        Silo& output_silo_;
        int production_rate_;
        int tier_;

    public:
        Factory(std::vector<Silo*> input_silos, std::vector<int> input_volumes, Silo& output_silo, 
                std::barrier<>& sim_barrier, int production_rate, std::atomic<bool>& is_running, int max_phases, int tier);       
};