#include <resource_collectors/resource_collector.hpp>

ResourceCollector::ResourceCollector(Silo& output_silo, std::barrier<>& sim_barrier, int production_rate, std::atomic<bool>& is_running)
:   output_silo_(output_silo),
    sim_barrier_(sim_barrier),
    production_rate_(production_rate),
    is_running_(is_running)
    {}

void ResourceCollector::Start()
{
    collector_thread_ = std::thread(&ResourceCollector::Run, this);  
}

void ResourceCollector::Run()
{
    while (true)
    {
        sim_barrier_.arrive_and_wait();
        if (!is_running_)
        {
            sim_barrier_.arrive_and_wait();
            break;
        }
        output_silo_.AddResources(production_rate_);

        sim_barrier_.arrive_and_wait();

        
    }
}


void ResourceCollector::Join()
{
    if (collector_thread_.joinable())
        collector_thread_.join();
}

