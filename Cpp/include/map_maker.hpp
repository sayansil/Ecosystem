#include <organism.hpp>
#include <animal.hpp>
#include <plant.hpp>
#include <ecosystem_types.hpp>
#include <refl.hpp>

struct map_maker
{
    template <typename T>
    ATTRIBUTE_MAP_TYPE var_map_banana(T&& value)
    {
        ATTRIBUTE_MAP_TYPE ret_map;
        refl::util::for_each(refl::reflect(value).members, [&](auto member){
            if constexpr(refl::descriptor::is_field(member))
            {
                if(refl::descriptor::has_attribute<fixed_for_species>(member))
                {
                    std::string name = refl::descriptor::get_display_name(member);
                    ret_map["fixed_for_species"][name] = PStat(member(value));
                }
                else if(refl::descriptor::has_attribute<fixed_for_lifetime>(member))
                {
                    std::string name = refl::descriptor::get_display_name(member);
                    ret_map["fixed_for_lifetime"][name] = PStat(member(value));
                }
                else if(refl::descriptor::has_attribute<changes_every_year>(member))
                {
                    std::string name = refl::descriptor::get_display_name(member);
                    ret_map["changes_every_year"][name] = PStat(member(value));
                }
                else if(refl::descriptor::has_attribute<changes_every_moment>(member))
                {
                    std::string name = refl::descriptor::get_display_name(member);
                    ret_map["changes_every_moment"][name] = PStat(member(value));
                }
            }
        });
        return ret_map;
    }

    template <typename T>
    ATTRIBUTE_RAW_MAP_TYPE raw_var_map_banana(T &&value)
    {
        ATTRIBUTE_RAW_MAP_TYPE ret_map;
        refl::util::for_each(refl::reflect(value).members, [&](auto member) {
            if constexpr (refl::descriptor::is_field(member))
            {
                std::string name = refl::descriptor::get_display_name(member);
                ret_map[name] = PStat(member(value));
            }
        });
        return ret_map;
    }
};
