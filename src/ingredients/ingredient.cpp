#include <ingredients/ingredient.hpp>

Ingredient::Ingredient(Silo* input_silo)
{
    ingredient_silo_ = input_silo;
}

bool Ingredient::IsReadyToProcess()
{
    if(stored_units_ < units_for_processing_)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Ingredient::CollectResource()
{
    int available_units = ingredient_silo_->GetStoredUnits();
    int desired_units = units_for_processing_-stored_units_;

    if(available_units >= desired_units)
    {
        ingredient_silo_->TakeResources(desired_units);
        stored_units_+=desired_units;
    }
    else
    {
        ingredient_silo_->TakeResources(available_units);
        stored_units_+=available_units;
    }
}

void Ingredient::ClearStorage()
{
    stored_units_=0;
}