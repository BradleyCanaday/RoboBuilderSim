#include <factories/factory.hpp>

Factory::Factory(std::vector<Silo*> input_silos, std::vector<int> input_volumes, Silo& output_silo, 
        std::barrier<>& sim_barrier, int production_rate, std::atomic<bool>& is_running)
:   input_silos_(input_silos),
    has_input_(input_silos_.size(), false),
    input_volumes_(input_volumes),
    output_silo_(output_silo),
    sim_barrier_(sim_barrier),
    production_rate_(production_rate),
    is_running_(is_running)
    {}

void Factory::Start()
{
    collector_thread_ = std::thread(&Factory::Run, this);  
}

void Factory::Run()
{
    while (true)
    {
        sim_barrier_.arrive_and_wait();
        sim_barrier_.arrive_and_wait();
        if (!is_running_)
        {
            sim_barrier_.arrive_and_wait();
            break;
        }
        bool ready_to_produce = true;
        bool got_resource = false;

        for(int i=0;i<has_input_.size();i++)
        {
            if(has_input_[i] == false)
            {
                ready_to_produce = false;

                got_resource = input_silos_[i]->TakeResources(input_volumes_[i]);
                if(got_resource == true)
                {
                    has_input_[i] = true;
                    ready_to_produce = true;
                }
            }
        }

        if(ready_to_produce == true)
        {
            output_silo_.AddResources(production_rate_);
            for(int i=0;i<has_input_.size();i++)
            {
                has_input_[i] = false;
            }
        }

        sim_barrier_.arrive_and_wait();
        
    }
}


void Factory::Join()
{
    if (collector_thread_.joinable())
        collector_thread_.join();
}