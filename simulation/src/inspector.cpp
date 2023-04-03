#include <fmt/core.h>
#include <world_generated.h>

#include <inspector.hpp>

EcosystemInspector::EcosystemInspector(const uint8_t *input_buffer) {
    buffer = input_buffer;  // intentional shallow copy
    world_type_table = Ecosystem::WorldTypeTable();
    species_type_table = Ecosystem::SpeciesTypeTable();
    organism_type_table = Ecosystem::OrganismTypeTable();
}

void EcosystemInspector::IterateWorld(flatbuffers::ToStringVisitor *visitor) {
    using namespace flatbuffers;
    std::vector<nlohmann::json> species_json_list;
    const uint8_t *obj = flatbuffers::GetRoot<uint8_t>(buffer);
    visitor->StartSequence();
    const uint8_t *prev_val = nullptr;
    size_t set_idx = 0;
    size_t array_idx = 0;
    for (size_t i = 0; i < world_type_table->num_elems; i++) {
        auto type_code = world_type_table->type_codes[i];
        auto type = static_cast<ElementaryType>(type_code.base_type);
        auto is_repeating = type_code.is_repeating != 0;
        auto ref_idx = type_code.sequence_ref;
        const TypeTable *ref = nullptr;
        if (ref_idx >= 0) {
            ref = world_type_table->type_refs[ref_idx]();
        }
        auto name =
            world_type_table->names ? world_type_table->names[i] : nullptr;
        const uint8_t *val = nullptr;
        if (world_type_table->st == ST_TABLE) {
            val = reinterpret_cast<const Table *>(obj)->GetAddressOf(
                FieldIndexToOffset(static_cast<voffset_t>(i)));
        } else {
            val = obj + world_type_table->values[i];
        }
        if (std::string(name) != "species") {
            visitor->Field(i, set_idx, type, is_repeating, ref, name, val);
        }
        if (val) {
            set_idx++;
            if (is_repeating) {
                const uint8_t *elem_ptr = val;
                size_t size = 0;
                if (world_type_table->st == ST_TABLE) {
                    // variable length vector
                    val += ReadScalar<uoffset_t>(val);
                    auto vec = reinterpret_cast<const Vector<uint8_t> *>(val);
                    elem_ptr = vec->Data();
                    size = vec->size();
                } else {
                    // otherwise fixed size array
                    size = world_type_table->array_sizes[array_idx];
                    ++array_idx;
                }
                if (std::string(name) != "species") {
                    visitor->StartVector();
                }
                for (size_t j = 0; j < size; j++) {
                    if (std::string(name) != "species") {
                        visitor->Element(j, type, ref, elem_ptr);
                    }
                    if (std::string(name) == "species") {
                        species_json_list.emplace_back(IterateSpecies(
                            elem_ptr, ref,
                            ToStringVisitor("", true, "", false)));
                    } else {
                        IterateValue(type, elem_ptr, ref, prev_val,
                                     static_cast<soffset_t>(j), visitor);
                    }
                    elem_ptr += InlineSize(type, ref);
                }
                if (std::string(name) != "species") {
                    visitor->EndVector();
                }
            } else {
                IterateValue(type, val, ref, prev_val, -1, visitor);
            }
        }
        prev_val = val;
    }
    visitor->EndSequence();
    output = nlohmann::json::parse(visitor->s);
    for (const auto &species_json : species_json_list) {
        output["species"].emplace_back(species_json);
    }
}

nlohmann::json EcosystemInspector::IterateSpecies(
    const uint8_t *obj, const flatbuffers::TypeTable *type_table,
    flatbuffers::ToStringVisitor visitor) {
    using namespace flatbuffers;
    std::vector<nlohmann::json> organism_json_list;
    obj += ReadScalar<uoffset_t>(obj);
    visitor.StartSequence();
    const uint8_t *prev_val = nullptr;
    size_t set_idx = 0;
    size_t array_idx = 0;
    for (size_t i = 0; i < type_table->num_elems; i++) {
        auto type_code = type_table->type_codes[i];
        auto type = static_cast<ElementaryType>(type_code.base_type);
        auto is_repeating = type_code.is_repeating != 0;
        auto ref_idx = type_code.sequence_ref;
        const TypeTable *ref = nullptr;
        if (ref_idx >= 0) {
            ref = type_table->type_refs[ref_idx]();
        }
        auto name = type_table->names ? type_table->names[i] : nullptr;
        const uint8_t *val = nullptr;
        if (type_table->st == ST_TABLE) {
            val = reinterpret_cast<const Table *>(obj)->GetAddressOf(
                FieldIndexToOffset(static_cast<voffset_t>(i)));
        } else {
            val = obj + type_table->values[i];
        }
        if (std::string(name) != "organism") {
            visitor.Field(i, set_idx, type, is_repeating, ref, name, val);
        }
        if (val) {
            set_idx++;
            if (is_repeating) {
                auto elem_ptr = val;
                size_t size = 0;
                if (type_table->st == ST_TABLE) {
                    val += ReadScalar<uoffset_t>(val);
                    auto vec = reinterpret_cast<const Vector<uint8_t> *>(val);
                    elem_ptr = vec->Data();
                    size = vec->size();
                } else {
                    size = type_table->array_sizes[array_idx];
                    ++array_idx;
                }
                if (std::string(name) != "organism") {
                    visitor.StartVector();
                }
                for (size_t j = 0; j < size; j++) {
                    if (std::string(name) != "organism") {
                        visitor.Element(j, type, ref, elem_ptr);
                    }
                    if (std::string(name) == "organism") {
                        organism_json_list.emplace_back(IterateOrganism(
                            elem_ptr, ref,
                            ToStringVisitor("", true, "", false)));
                    } else {
                        IterateValue(type, elem_ptr, ref, prev_val,
                                     static_cast<soffset_t>(j), &visitor);
                    }
                    elem_ptr += InlineSize(type, ref);
                }
                if (std::string(name) != "organism") {
                    visitor.EndVector();
                }
            } else {
                IterateValue(type, val, ref, prev_val, -1, &visitor);
            }
        }
        prev_val = val;
    }
    visitor.EndSequence();
    auto species_json = nlohmann::json::parse(visitor.s);
    for (const auto &organism_json : organism_json_list) {
        species_json["organism"].emplace_back(organism_json);
    }
    return species_json;
}

nlohmann::json EcosystemInspector::IterateOrganism(
    const uint8_t *obj, const flatbuffers::TypeTable *type_table,
    flatbuffers::ToStringVisitor visitor) {
    using namespace flatbuffers;
    obj += ReadScalar<uoffset_t>(obj);
    visitor.StartSequence();
    const uint8_t *prev_val = nullptr;
    size_t set_idx = 0;
    size_t array_idx = 0;
    for (size_t i = 0; i < type_table->num_elems; i++) {
        auto type_code = type_table->type_codes[i];
        auto type = static_cast<ElementaryType>(type_code.base_type);
        auto is_repeating = type_code.is_repeating != 0;
        auto ref_idx = type_code.sequence_ref;
        const TypeTable *ref = nullptr;
        if (ref_idx >= 0) {
            ref = type_table->type_refs[ref_idx]();
        }
        auto name = type_table->names ? type_table->names[i] : nullptr;
        const uint8_t *val = nullptr;
        if (type_table->st == ST_TABLE) {
            val = reinterpret_cast<const Table *>(obj)->GetAddressOf(
                FieldIndexToOffset(static_cast<voffset_t>(i)));
        } else {
            val = obj + type_table->values[i];
        }
        visitor.Field(i, set_idx, type, is_repeating, ref, name, val);
        if (val) {
            set_idx++;
            if (is_repeating) {
                auto elem_ptr = val;
                size_t size = 0;
                if (type_table->st == ST_TABLE) {
                    // variable length vector
                    val += ReadScalar<uoffset_t>(val);
                    auto vec = reinterpret_cast<const Vector<uint8_t> *>(val);
                    elem_ptr = vec->Data();
                    size = vec->size();
                } else {
                    // otherwise fixed size array
                    size = type_table->array_sizes[array_idx];
                    ++array_idx;
                }
                visitor.StartVector();
                for (size_t j = 0; j < size; j++) {
                    visitor.Element(j, type, ref, elem_ptr);
                    IterateValue(type, elem_ptr, ref, prev_val,
                                 static_cast<soffset_t>(j), &visitor);
                    elem_ptr += InlineSize(type, ref);
                }
                visitor.EndVector();
            } else {
                IterateValue(type, val, ref, prev_val, -1, &visitor);
            }
        }
        prev_val = val;
    }
    visitor.EndSequence();
    return nlohmann::json::parse(visitor.s);
}
