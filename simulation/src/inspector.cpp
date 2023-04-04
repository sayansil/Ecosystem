#include <fmt/core.h>
#include <fmt/ranges.h>
#include <world_generated.h>

#include <inspector.hpp>

EcosystemInspector::EcosystemInspector(
    const uint8_t *input_buffer, const nlohmann::ordered_json &input_query) {
    buffer = input_buffer;  // intentional shallow copy
    world_type_table = Ecosystem::WorldTypeTable();
    species_type_table = Ecosystem::SpeciesTypeTable();
    organism_type_table = Ecosystem::OrganismTypeTable();
    query = input_query;
}

void EcosystemInspector::IterateWorld() {
    using namespace flatbuffers;
    ToStringVisitor visitor("", true, "", false);
    std::vector<nlohmann::json> species_json_list;
    const uint8_t *obj = flatbuffers::GetRoot<uint8_t>(buffer);
    visitor.StartSequence();
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
            visitor.Field(i, set_idx, type, is_repeating, ref, name, val);
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
                    visitor.StartVector();
                }
                for (size_t j = 0; j < size; j++) {
                    if (std::string(name) != "species") {
                        visitor.Element(j, type, ref, elem_ptr);
                    }
                    if (std::string(name) == "species") {
                        species_json_list.emplace_back(IterateSpecies(
                            elem_ptr, ref,
                            ToStringVisitor("", true, "", false)));
                    } else {
                        IterateValue(type, elem_ptr, ref, prev_val,
                                     static_cast<soffset_t>(j), &visitor);
                    }
                    elem_ptr += InlineSize(type, ref);
                }
                if (std::string(name) != "species") {
                    visitor.EndVector();
                }
            } else {
                IterateValue(type, val, ref, prev_val, -1, &visitor);
            }
        }
        prev_val = val;
    }
    visitor.EndSequence();
    output = nlohmann::json::parse(visitor.s);
    for (const auto &species_json : species_json_list) {
        output["species"].emplace_back(species_json);
    }
}

int EcosystemInspector::processSpeciesQuery(
    const nlohmann::ordered_json &species_queries,
    flatbuffers::ToStringVisitor visitor) {
    visitor.EndSequence();
    nlohmann::ordered_json species_json =
        nlohmann::json::parse(std::move(visitor.s));
    size_t index = 0;
    for (auto species_query_item : species_queries) {
        if (species_query_item["organism"] != nullptr) {
            species_query_item.erase("organism");
        }
        if (species_json == species_query_item) {
            return index;
        }
        index++;
    }
    return -1;
}

bool EcosystemInspector::processOrganismQuery(
    const nlohmann::json &organism_queries,
    const nlohmann::json &organism_json) {
    for (const auto &index : organism_queries.items()) {
        for (const auto &value : index.value().items()) {
            if (value.value().is_structured()) {
                float lower_bound = std::numeric_limits<float>::min();
                float upper_bound = std::numeric_limits<float>::max();
                std::string organism_stat = value.key();
                for (const auto &stat_bounds : value.value().items()) {
                    if (stat_bounds.key() == "low") {
                        lower_bound = stat_bounds.value().get<float>();
                    }
                    if (stat_bounds.key() == "high") {
                        upper_bound = stat_bounds.value().get<float>();
                    }
                }
                if (organism_json[value.key()] < lower_bound ||
                    organism_json[value.key()] > upper_bound) {
                    return false;
                }
            }
        }
    }
    return true;
}

nlohmann::json EcosystemInspector::IterateSpecies(
    const uint8_t *obj, const flatbuffers::TypeTable *type_table,
    flatbuffers::ToStringVisitor visitor) {
    using namespace flatbuffers;
    std::vector<nlohmann::json> organism_json_list;
    nlohmann::json extracted_organism_stats;
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
                        if (int matchIndex =
                                processSpeciesQuery(query["species"], visitor);
                            matchIndex != -1) {
                            auto organism_json = IterateOrganism(
                                elem_ptr, ref,
                                ToStringVisitor("", true, "", false));
                            if (query["type"] == "extract") {
                                for (const auto &query_organism_stat :
                                     query["species"][matchIndex]["organism"]
                                         .items()) {
                                    extracted_organism_stats[query_organism_stat
                                                                 .value()]
                                        .push_back(
                                            organism_json[query_organism_stat
                                                              .value()]);
                                }
                            } else if (query["type"] == "filter") {
                                if (processOrganismQuery(
                                        query["species"][matchIndex]
                                             ["organism"],
                                        organism_json)) {
                                    organism_json_list.emplace_back(
                                        std::move(organism_json));
                                }
                            }
                        }

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
    if (query["type"] == "filter") {
        for (const auto &organism_json : organism_json_list) {
            species_json["organism"].emplace_back(organism_json);
        }
    } else if (query["type"] == "extract") {
        species_json["organism"] = std::move(extracted_organism_stats);
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
