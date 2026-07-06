#pragma once

#include <string>
#include <map>

struct Blueprint {
    std::string name;
    std::map<std::string, int> allowed_input_ports; 
    std::string output_type;
};