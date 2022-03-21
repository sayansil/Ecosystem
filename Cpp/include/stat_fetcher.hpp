#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <species_report_generated.h>
#include <world_generated.h>
#include <ecosystem_types.hpp>

namespace stat_fetcher
{
    extern flatbuffers::DetachedBuffer export_report(
        const std::string &kind,
        const std::string &kingdom,
        const std::string &title);
        
    extern FBuffer create_avg_world(
        const flatbuffers::DetachedBuffer &world_buffer
    );

}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
