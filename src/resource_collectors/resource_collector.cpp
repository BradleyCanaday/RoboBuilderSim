#include <resource_collectors/resource_collector.hpp>

ResourceCollector::ResourceCollector(Silo& output_silo,
                                     std::barrier<>& sim_barrier,
                                     int production_rate,
                                     std::atomic<bool>& is_running,
                                     int max_phases)
    : SimulationWorker(sim_barrier, is_running, max_phases)
    , output_silo_(output_silo)
    , production_rate_(production_rate)
{}

void ResourceCollector::Step(int current_phase)
{
    if(current_phase == 0)
    {
        output_silo_.AddResources(production_rate_);
    }
    
}

