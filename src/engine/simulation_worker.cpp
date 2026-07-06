#include <engine/simulation_worker.hpp>

SimulationWorker::SimulationWorker(std::barrier<>& barrier,
                                   std::atomic<bool>& running)
    : barrier_(barrier), running_(running)
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
        barrier_.arrive_and_wait();   // Phase 1

        if (!running_)
        {
            barrier_.arrive_and_wait();
            barrier_.arrive_and_wait();
            break;
        }
        StepPhase1();
        barrier_.arrive_and_wait();   // Phase 2
        
        StepPhase2();
        barrier_.arrive_and_wait();   // Phase 3
        
    }
}