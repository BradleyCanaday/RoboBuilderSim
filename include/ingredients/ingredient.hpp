#pragma once

#include <ingredients/silo.hpp>

class Ingredient
{
    private:
        Silo* ingredient_silo_;
        int stored_units_;
        int units_for_processing_;

    public:
        Ingredient(Silo input_silo);

        bool IsReadyToProcess();
        void CollectResource();

        void ClearStorage();
};