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

#endif // ECOSYSTEMTYPES_HPP
