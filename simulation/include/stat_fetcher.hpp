#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <world_generated.h>
#include <ecosystem_types.hpp>
#include <unordered_map>

namespace stat_fetcher
{
    extern FBuffer create_avg_world(const flatbuffers::DetachedBuffer &world_buffer);
    extern std::unordered_map<uint32_t, uint32_t> get_generation_count(const flatbuffers::DetachedBuffer &world_buffer);
    extern std::unordered_map<uint32_t, uint32_t> get_age_count(const flatbuffers::DetachedBuffer &world_buffer);

}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
