#pragma once

#include <mutex>

class Silo
{
    private:
        mutable std::mutex silo_mutex_;
        int stored_units_;

    public:
        Silo();

        void AddResources(int resource_units);
        bool TakeResources(int resource_units);
        int GetStoredUnits() const;
};