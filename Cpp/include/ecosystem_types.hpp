#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

using FBuffer = std::vector<uint8_t>;

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
