#include <factories/factory.hpp>

Factory::Factory(std::vector<Silo*> input_silos,
                 std::vector<int> input_volumes,
                 Silo& output_silo,
                 std::barrier<>& sim_barrier,
                 int production_rate,
                 std::atomic<bool>& running,
                 int max_phases,
                 int tier)
    : SimulationWorker(sim_barrier, running, max_phases)
    , input_silos_(std::move(input_silos))
    , input_volumes_(std::move(input_volumes))
    , has_input_(input_silos_.size(), false)
    , output_silo_(output_silo)
    , production_rate_(production_rate)
    , tier_(tier)
{}

void Factory::Step(int current_phase)
{
    if(current_phase == tier_)
    {
        bool ready = true;

        for (size_t i = 0; i < input_silos_.size(); i++)
        {
            if (!has_input_[i])
            {
                if (input_silos_[i]->TakeResources(input_volumes_[i]))
                    has_input_[i] = true;
                else
                    ready = false;
            }
        }

        if (ready)
        {
            output_silo_.AddResources(production_rate_);
            std::fill(has_input_.begin(), has_input_.end(), false);
        }
    }
}