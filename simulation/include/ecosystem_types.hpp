#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <flatbuffers/flatbuffers.h>

using FBuffer = flatbuffers::DetachedBuffer;
using ByteArray = std::vector<uint8_t>;

struct FBufferView {
    uint8_t * data;
    size_t size;

    FBufferView() = default;
    FBufferView(uint8_t * data, size_t size) : data(data), size(size) {}
};

namespace EcosystemTypes
{
    static const std::vector<std::string> get_kingdom_name = {
        "animal",
        "plant"};

    static const std::vector<std::string> get_sexuality = {
        "sexual",
        "asexual"};
};

#endif // ECOSYSTEMTYPES_HPP
