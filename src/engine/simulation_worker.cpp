#include <engine/simulation_worker.hpp>

SimulationWorker::SimulationWorker(std::barrier<>& barrier,
                                   std::atomic<bool>& running,
                                   int max_phases)
    : barrier_(barrier), running_(running), max_phases_(max_phases)
{}

void SimulationWorker::Start()
{
    thread_ = std::thread(&SimulationWorker::Run, this);
}

void SimulationWorker::Join()
{
    if (thread_.joinable())
        thread_.join();
}

void SimulationWorker::Run()
{
    while (true)
    {
        for (int phase = 0; phase < max_phases_; ++phase)
        {
            barrier_.arrive_and_wait();

            if (running_) {
                Step(phase); 
            }
        }

        if(!running_)
        {
            break;
        }
    }
}