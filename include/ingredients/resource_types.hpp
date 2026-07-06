#pragma once
#include <string>

struct Resource {
    std::string id;
    std::string name;
    int output_per_tick;
    int tier;
};