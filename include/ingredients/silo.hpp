#pragma once

#include <mutex>

class Silo
{
    private:
        std::mutex silo_mutex_;
        int stored_units_;

    public:
        Silo();

        void AddResources(int resource_units);
        void TakeResources(int resource_units);
        int GetStoredUnits();
};