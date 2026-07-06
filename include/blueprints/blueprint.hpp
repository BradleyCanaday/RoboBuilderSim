#pragma once

#include <vector>
#include <string>
#include <map>

#include <ingredients/resource_types.hpp>

struct Blueprint {
    std::string name;
    
    std::map<Resource, int> allowed_input_ports; 
    
    Resource output_type;
    int output_amount;
};