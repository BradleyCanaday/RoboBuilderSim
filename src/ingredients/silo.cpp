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
void Silo::TakeResources(int resource_units)
{
    std::lock_guard<std::mutex> lock(silo_mutex_);
    stored_units_-=resource_units;
}
int Silo::GetStoredUnits()
{
    return stored_units_;
}

        