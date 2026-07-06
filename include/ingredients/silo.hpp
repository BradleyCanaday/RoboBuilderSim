#pragma once

#include <mutex>
#include <string>

class Silo
{
    private:
        mutable std::mutex silo_mutex_;
        int stored_units_;
        std::string resource_id_;

    public:
        Silo(std::string resource_id);

        void AddResources(int resource_units);
        bool TakeResources(int resource_units);
        int GetStoredUnits() const;
        std::string GetId() const;
};