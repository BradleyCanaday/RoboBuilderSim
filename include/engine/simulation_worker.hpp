#pragma once

#include <thread>
#include <barrier>
#include <atomic>

class SimulationWorker
{
    protected:
        std::thread thread_;
        std::barrier<>& barrier_;
        std::atomic<bool>& running_;
        int max_phases_;

        virtual void Step(int current_phase) = 0; 

    public:
        SimulationWorker(std::barrier<>& barrier, std::atomic<bool>& running, int max_phases);
        virtual ~SimulationWorker() = default;

        void Start();
        void Join();
        
    private:
        void Run();
};