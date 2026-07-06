#include <ingredients/silo.hpp>

Silo::Silo(std::string resource_id)
{
    resource_id_ = resource_id;
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
int Silo::GetStoredUnits() const
{
    std::lock_guard<std::mutex> lock(silo_mutex_);
    return stored_units_;
}

std::string Silo::GetId() const
{
    return resource_id_;
}

        