#pragma once

#include <atomic>
#include <barrier>
#include <thread>

class SimulationWorker
{
    public:
        virtual ~SimulationWorker() = default;

        void Start();
        void Join();

    protected:
        SimulationWorker(std::barrier<>& barrier,
                         std::atomic<bool>& running);

        virtual void StepPhase1() {}
        virtual void StepPhase2() {}

        std::barrier<>& barrier_;
        std::atomic<bool>& running_;
        std::thread thread_;

    private:
        void Run();
};