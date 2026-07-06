#include <ingredients/silo.hpp>

Silo::Silo()
{
    stored_units_ = 0;
}

void Silo::AddResources(int resource_units)
{
    std::lock_guard<std::mutex> lock(silo_mutex_);
    stored_units_+=resource_units;
}
bool Silo::TakeResources(int resource_units)
{
    std::lock_guard<std::mutex> lock(silo_mutex_);
    if(stored_units_-resource_units >= 0)
    {
        stored_units_=stored_units_-resource_units;
        return true;
    }
    else
    {
        return false;
    }
}
int Silo::GetStoredUnits()
{
    std::lock_guard<std::mutex> lock(silo_mutex_);
    return stored_units_;
}

        