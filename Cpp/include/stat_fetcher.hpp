#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <species_report_generated.h>
#include <world_generated.h>

namespace stat_fetcher
{
    extern flatbuffers::DetachedBuffer export_report(
        const flatbuffers::DetachedBuffer &world_buffer,
        const std::string &kind,
        const std::string &kingdom,
        const std::string &title);

}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
