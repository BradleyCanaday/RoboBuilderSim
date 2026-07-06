#pragma once

#include <atomic>
#include <barrier>

#include <engine/simulation_worker.hpp>
#include <ingredients/silo.hpp>

class ResourceCollector: public SimulationWorker
{
    private:
        Silo& output_silo_;
        int production_rate_;

        void Step(int current_phase) override;

    public:
        ResourceCollector(Silo& output_silo, std::barrier<>& sim_barrier, int production_rate, std::atomic<bool>& is_running, int max_phases);       
};