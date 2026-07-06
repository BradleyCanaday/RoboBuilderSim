#pragma once

#include <array>
#include <cstddef>

enum class Resource
{
    Iron,
    Coal,
    Copper,
    Steel,
    Wire,

    Count
};

constexpr std::size_t ToIndex(Resource resource)
{
    return static_cast<std::size_t>(resource);
}

struct ResourceInfo
{
    std::string_view name;
    int output_per_tick;
};

inline constexpr std::array<ResourceInfo, ToIndex(Resource::Count)> resource_info{{
    {"Iron", 10},
    {"Coal", 5},
    {"Copper", 10},
    {"Steel", 1},
    {"Wire", 1},
}};

constexpr const ResourceInfo& GetResourceInfo(Resource resource)
{
    return resource_info[ToIndex(resource)];
}