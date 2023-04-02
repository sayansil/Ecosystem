#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <population_generated.h>

#include <database_manager.hpp>
#include <ecosystem_types.hpp>
#include <god.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <vector>

void print_attribute_list (const flatbuffers::TypeTable* type_table) {
    std::string attribute_list_string;
    for (size_t i = 0; i < type_table->num_elems; i++) {
        attribute_list_string += type_table->names[i];
        attribute_list_string += '\n';
    }
    fmt::print("{}", attribute_list_string);
}

void print_organisms(const uint8_t *obj, const flatbuffers::TypeTable *type_table,
        flatbuffers::IterationVisitor *visitor) {
  visitor->StartSequence();
  const uint8_t *prev_val = nullptr;
  size_t set_idx = 0;
  size_t array_idx = 0;
  for (size_t i = 0; i < type_table->num_elems; i++) {
    auto type_code = type_table->type_codes[i];
    auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
    auto is_repeating = type_code.is_repeating != 0;
    const flatbuffers::TypeTable *ref = Ecosystem::OrganismTypeTable();
    auto name = type_table->names ? type_table->names[i] : nullptr;
    const uint8_t *val = nullptr;
    if (type_table->st == flatbuffers::ST_TABLE) {
      val = reinterpret_cast<const flatbuffers::Table *>(obj)->GetAddressOf(
          flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(i)));
    } else {
      val = obj + type_table->values[i];
    }
    visitor->Field(i, set_idx, type, is_repeating, ref, name, val);
    if (val) {
      set_idx++;
      if (is_repeating) {
        auto elem_ptr = val;
        size_t size = 0;
        if (type_table->st == flatbuffers::ST_TABLE) {
          // variable length vector
          val += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(val);
          auto vec = reinterpret_cast<const flatbuffers::Vector<uint8_t> *>(val);
          elem_ptr = vec->Data();
          size = vec->size();
        } else {
          // otherwise fixed size array
          size = type_table->array_sizes[array_idx];
          ++array_idx;
        }
        visitor->StartVector();
        for (size_t j = 0; j < size; j++) {
          visitor->Element(j, type, ref, elem_ptr);
          IterateValue(type, elem_ptr, ref, prev_val, static_cast<flatbuffers::soffset_t>(j),
                       visitor);
          elem_ptr += InlineSize(type, ref);
        }
        visitor->EndVector();
      } else {
        IterateValue(type, val, ref, prev_val, -1, visitor);
      }
    }
    prev_val = val;
  }
  visitor->EndSequence();
}

void iterate_over_organisms(flatbuffers::ElementaryType type, const uint8_t *species_ptr,
                         const flatbuffers::TypeTable *ref, const uint8_t *prev_species,
                         flatbuffers::soffset_t vector_index, flatbuffers::IterationVisitor *visitor) {
    species_ptr += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(species_ptr);
    //IterateObject(species_ptr, ref, visitor);
    print_organisms(species_ptr, ref, visitor);
}

void print_species(const uint8_t *buffer) {
    flatbuffers::ToStringVisitor visitor("", true, "", false);
    visitor.StartVector();
    const flatbuffers::TypeTable* type_table = Ecosystem::WorldTypeTable();
    const uint8_t* root = flatbuffers::GetRoot<uint8_t>(buffer);
    const uint8_t* prev_species = nullptr;
    auto type_code = type_table->type_codes[type_table->num_elems - 1];
    auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
    auto is_repeating = type_code.is_repeating != 0;
    const flatbuffers::TypeTable *ref = Ecosystem::SpeciesTypeTable();
    auto name = type_table->names ? type_table->names[type_table->num_elems - 1] : nullptr;
    const uint8_t* species = reinterpret_cast<const flatbuffers::Table *>(root)->GetAddressOf(
          flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(type_table->num_elems - 1)));
    auto species_ptr = species;
    size_t species_size = 0;

    species += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(species);
    auto species_vec = reinterpret_cast<const flatbuffers::Vector<uint8_t> *>(species);
    species_ptr = species_vec->Data();
    species_size = species_vec->size();
    for (size_t j = 0; j < species_size; j++) {
        visitor.Element(j, type, ref, species_ptr);
        iterate_over_organisms(type, species_ptr, ref, prev_species, static_cast<flatbuffers::soffset_t>(j), &visitor);
        species_ptr += InlineSize(type, ref);
    }
    visitor.EndVector();
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("{}\n", json_data.dump());
    //fmt::print("{}\n", visitor.s);
}

int main() {
    std::vector<std::vector<ByteArray>> rows;
    const size_t simulation_years = 5;

    setup::setup(helper::get_ecosystem_root());

    const size_t initial_organism_count = 8;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count / 2; i++) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
    }

    for (size_t i = 0; i < initial_organism_count / 2; i++) {
        organisms.push_back(
            {{"kind", "bamboo"}, {"kingdom", "1"}, {"age", "20"}});
    }

    {
        God allah(helper::get_ecosystem_root(), true);
        allah.cleanSlate();
        allah.createWorld(organisms);
        for (size_t i = 0; i < simulation_years; i++) {
            allah.happy_new_year(true);
        }

        FBuffer avg_world = stat_fetcher::create_avg_world(allah.buffer);
        //print_attribute_list(Ecosystem::OrganismTypeTable());
        print_species(allah.buffer.data());
    }
}
