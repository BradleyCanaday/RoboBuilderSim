#include <resource_collectors/resource_collector.hpp>

ResourceCollector::ResourceCollector(Silo& output_silo,
                                     std::barrier<>& sim_barrier,
                                     int production_rate,
                                     std::atomic<bool>& is_running)
    : SimulationWorker(sim_barrier, is_running)
    , output_silo_(output_silo)
    , production_rate_(production_rate)
{}

void ResourceCollector::StepPhase1()
{
    output_silo_.AddResources(production_rate_);
}

