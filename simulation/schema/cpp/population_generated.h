// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_POPULATION_ECOSYSTEM_H_
#define FLATBUFFERS_GENERATED_POPULATION_ECOSYSTEM_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 22 &&
              FLATBUFFERS_VERSION_MINOR == 9 &&
              FLATBUFFERS_VERSION_REVISION == 29,
             "Non-compatible flatbuffers version included");

namespace Ecosystem {

struct RawPopulation;

struct SpeciesPopulation;
struct SpeciesPopulationBuilder;
struct SpeciesPopulationT;

struct WorldPopulation;
struct WorldPopulationBuilder;
struct WorldPopulationT;

inline const flatbuffers::TypeTable *RawPopulationTypeTable();

inline const flatbuffers::TypeTable *SpeciesPopulationTypeTable();

inline const flatbuffers::TypeTable *WorldPopulationTypeTable();

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) RawPopulation FLATBUFFERS_FINAL_CLASS {
 private:
  uint32_t male_population_;
  uint32_t female_population_;

 public:
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return RawPopulationTypeTable();
  }
  RawPopulation()
      : male_population_(0),
        female_population_(0) {
  }
  RawPopulation(uint32_t _male_population, uint32_t _female_population)
      : male_population_(flatbuffers::EndianScalar(_male_population)),
        female_population_(flatbuffers::EndianScalar(_female_population)) {
  }
  uint32_t male_population() const {
    return flatbuffers::EndianScalar(male_population_);
  }
  void mutate_male_population(uint32_t _male_population) {
    flatbuffers::WriteScalar(&male_population_, _male_population);
  }
  uint32_t female_population() const {
    return flatbuffers::EndianScalar(female_population_);
  }
  void mutate_female_population(uint32_t _female_population) {
    flatbuffers::WriteScalar(&female_population_, _female_population);
  }
};
FLATBUFFERS_STRUCT_END(RawPopulation, 8);

struct RawPopulation::Traits {
  using type = RawPopulation;
};

struct SpeciesPopulationT : public flatbuffers::NativeTable {
  typedef SpeciesPopulation TableType;
  std::string kind{};
  std::unique_ptr<Ecosystem::RawPopulation> matable_population{};
  std::unique_ptr<Ecosystem::RawPopulation> non_matable_population{};
  SpeciesPopulationT() = default;
  SpeciesPopulationT(const SpeciesPopulationT &o);
  SpeciesPopulationT(SpeciesPopulationT&&) FLATBUFFERS_NOEXCEPT = default;
  SpeciesPopulationT &operator=(SpeciesPopulationT o) FLATBUFFERS_NOEXCEPT;
};

struct SpeciesPopulation FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef SpeciesPopulationT NativeTableType;
  typedef SpeciesPopulationBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return SpeciesPopulationTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KIND = 4,
    VT_MATABLE_POPULATION = 6,
    VT_NON_MATABLE_POPULATION = 8
  };
  const flatbuffers::String *kind() const {
    return GetPointer<const flatbuffers::String *>(VT_KIND);
  }
  flatbuffers::String *mutable_kind() {
    return GetPointer<flatbuffers::String *>(VT_KIND);
  }
  bool KeyCompareLessThan(const SpeciesPopulation *o) const {
    return *kind() < *o->kind();
  }
  int KeyCompareWithValue(const char *_kind) const {
    return strcmp(kind()->c_str(), _kind);
  }
  const Ecosystem::RawPopulation *matable_population() const {
    return GetStruct<const Ecosystem::RawPopulation *>(VT_MATABLE_POPULATION);
  }
  Ecosystem::RawPopulation *mutable_matable_population() {
    return GetStruct<Ecosystem::RawPopulation *>(VT_MATABLE_POPULATION);
  }
  const Ecosystem::RawPopulation *non_matable_population() const {
    return GetStruct<const Ecosystem::RawPopulation *>(VT_NON_MATABLE_POPULATION);
  }
  Ecosystem::RawPopulation *mutable_non_matable_population() {
    return GetStruct<Ecosystem::RawPopulation *>(VT_NON_MATABLE_POPULATION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_KIND) &&
           verifier.VerifyString(kind()) &&
           VerifyField<Ecosystem::RawPopulation>(verifier, VT_MATABLE_POPULATION, 4) &&
           VerifyField<Ecosystem::RawPopulation>(verifier, VT_NON_MATABLE_POPULATION, 4) &&
           verifier.EndTable();
  }
  SpeciesPopulationT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(SpeciesPopulationT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<SpeciesPopulation> Pack(flatbuffers::FlatBufferBuilder &_fbb, const SpeciesPopulationT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct SpeciesPopulationBuilder {
  typedef SpeciesPopulation Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_kind(flatbuffers::Offset<flatbuffers::String> kind) {
    fbb_.AddOffset(SpeciesPopulation::VT_KIND, kind);
  }
  void add_matable_population(const Ecosystem::RawPopulation *matable_population) {
    fbb_.AddStruct(SpeciesPopulation::VT_MATABLE_POPULATION, matable_population);
  }
  void add_non_matable_population(const Ecosystem::RawPopulation *non_matable_population) {
    fbb_.AddStruct(SpeciesPopulation::VT_NON_MATABLE_POPULATION, non_matable_population);
  }
  explicit SpeciesPopulationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<SpeciesPopulation> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<SpeciesPopulation>(end);
    fbb_.Required(o, SpeciesPopulation::VT_KIND);
    return o;
  }
};

inline flatbuffers::Offset<SpeciesPopulation> CreateSpeciesPopulation(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> kind = 0,
    const Ecosystem::RawPopulation *matable_population = nullptr,
    const Ecosystem::RawPopulation *non_matable_population = nullptr) {
  SpeciesPopulationBuilder builder_(_fbb);
  builder_.add_non_matable_population(non_matable_population);
  builder_.add_matable_population(matable_population);
  builder_.add_kind(kind);
  return builder_.Finish();
}

struct SpeciesPopulation::Traits {
  using type = SpeciesPopulation;
  static auto constexpr Create = CreateSpeciesPopulation;
};

inline flatbuffers::Offset<SpeciesPopulation> CreateSpeciesPopulationDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *kind = nullptr,
    const Ecosystem::RawPopulation *matable_population = nullptr,
    const Ecosystem::RawPopulation *non_matable_population = nullptr) {
  auto kind__ = kind ? _fbb.CreateString(kind) : 0;
  return Ecosystem::CreateSpeciesPopulation(
      _fbb,
      kind__,
      matable_population,
      non_matable_population);
}

flatbuffers::Offset<SpeciesPopulation> CreateSpeciesPopulation(flatbuffers::FlatBufferBuilder &_fbb, const SpeciesPopulationT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct WorldPopulationT : public flatbuffers::NativeTable {
  typedef WorldPopulation TableType;
  uint32_t year = 0;
  std::vector<std::unique_ptr<Ecosystem::SpeciesPopulationT>> species_population{};
  WorldPopulationT() = default;
  WorldPopulationT(const WorldPopulationT &o);
  WorldPopulationT(WorldPopulationT&&) FLATBUFFERS_NOEXCEPT = default;
  WorldPopulationT &operator=(WorldPopulationT o) FLATBUFFERS_NOEXCEPT;
};

struct WorldPopulation FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef WorldPopulationT NativeTableType;
  typedef WorldPopulationBuilder Builder;
  struct Traits;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return WorldPopulationTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_YEAR = 4,
    VT_SPECIES_POPULATION = 6
  };
  uint32_t year() const {
    return GetField<uint32_t>(VT_YEAR, 0);
  }
  bool mutate_year(uint32_t _year = 0) {
    return SetField<uint32_t>(VT_YEAR, _year, 0);
  }
  bool KeyCompareLessThan(const WorldPopulation *o) const {
    return year() < o->year();
  }
  int KeyCompareWithValue(uint32_t _year) const {
    return static_cast<int>(year() > _year) - static_cast<int>(year() < _year);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>> *species_population() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>> *>(VT_SPECIES_POPULATION);
  }
  flatbuffers::Vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>> *mutable_species_population() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>> *>(VT_SPECIES_POPULATION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_YEAR, 4) &&
           VerifyOffset(verifier, VT_SPECIES_POPULATION) &&
           verifier.VerifyVector(species_population()) &&
           verifier.VerifyVectorOfTables(species_population()) &&
           verifier.EndTable();
  }
  WorldPopulationT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(WorldPopulationT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<WorldPopulation> Pack(flatbuffers::FlatBufferBuilder &_fbb, const WorldPopulationT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct WorldPopulationBuilder {
  typedef WorldPopulation Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_year(uint32_t year) {
    fbb_.AddElement<uint32_t>(WorldPopulation::VT_YEAR, year, 0);
  }
  void add_species_population(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>>> species_population) {
    fbb_.AddOffset(WorldPopulation::VT_SPECIES_POPULATION, species_population);
  }
  explicit WorldPopulationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<WorldPopulation> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<WorldPopulation>(end);
    return o;
  }
};

inline flatbuffers::Offset<WorldPopulation> CreateWorldPopulation(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t year = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>>> species_population = 0) {
  WorldPopulationBuilder builder_(_fbb);
  builder_.add_species_population(species_population);
  builder_.add_year(year);
  return builder_.Finish();
}

struct WorldPopulation::Traits {
  using type = WorldPopulation;
  static auto constexpr Create = CreateWorldPopulation;
};

inline flatbuffers::Offset<WorldPopulation> CreateWorldPopulationDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t year = 0,
    std::vector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>> *species_population = nullptr) {
  auto species_population__ = species_population ? _fbb.CreateVectorOfSortedTables<Ecosystem::SpeciesPopulation>(species_population) : 0;
  return Ecosystem::CreateWorldPopulation(
      _fbb,
      year,
      species_population__);
}

flatbuffers::Offset<WorldPopulation> CreateWorldPopulation(flatbuffers::FlatBufferBuilder &_fbb, const WorldPopulationT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline SpeciesPopulationT::SpeciesPopulationT(const SpeciesPopulationT &o)
      : kind(o.kind),
        matable_population((o.matable_population) ? new Ecosystem::RawPopulation(*o.matable_population) : nullptr),
        non_matable_population((o.non_matable_population) ? new Ecosystem::RawPopulation(*o.non_matable_population) : nullptr) {
}

inline SpeciesPopulationT &SpeciesPopulationT::operator=(SpeciesPopulationT o) FLATBUFFERS_NOEXCEPT {
  std::swap(kind, o.kind);
  std::swap(matable_population, o.matable_population);
  std::swap(non_matable_population, o.non_matable_population);
  return *this;
}

inline SpeciesPopulationT *SpeciesPopulation::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::make_unique<SpeciesPopulationT>();
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void SpeciesPopulation::UnPackTo(SpeciesPopulationT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = kind(); if (_e) _o->kind = _e->str(); }
  { auto _e = matable_population(); if (_e) _o->matable_population = std::unique_ptr<Ecosystem::RawPopulation>(new Ecosystem::RawPopulation(*_e)); }
  { auto _e = non_matable_population(); if (_e) _o->non_matable_population = std::unique_ptr<Ecosystem::RawPopulation>(new Ecosystem::RawPopulation(*_e)); }
}

inline flatbuffers::Offset<SpeciesPopulation> SpeciesPopulation::Pack(flatbuffers::FlatBufferBuilder &_fbb, const SpeciesPopulationT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateSpeciesPopulation(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<SpeciesPopulation> CreateSpeciesPopulation(flatbuffers::FlatBufferBuilder &_fbb, const SpeciesPopulationT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const SpeciesPopulationT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _kind = _fbb.CreateString(_o->kind);
  auto _matable_population = _o->matable_population ? _o->matable_population.get() : nullptr;
  auto _non_matable_population = _o->non_matable_population ? _o->non_matable_population.get() : nullptr;
  return Ecosystem::CreateSpeciesPopulation(
      _fbb,
      _kind,
      _matable_population,
      _non_matable_population);
}

inline WorldPopulationT::WorldPopulationT(const WorldPopulationT &o)
      : year(o.year) {
  species_population.reserve(o.species_population.size());
  for (const auto &species_population_ : o.species_population) { species_population.emplace_back((species_population_) ? new Ecosystem::SpeciesPopulationT(*species_population_) : nullptr); }
}

inline WorldPopulationT &WorldPopulationT::operator=(WorldPopulationT o) FLATBUFFERS_NOEXCEPT {
  std::swap(year, o.year);
  std::swap(species_population, o.species_population);
  return *this;
}

inline WorldPopulationT *WorldPopulation::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::make_unique<WorldPopulationT>();
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void WorldPopulation::UnPackTo(WorldPopulationT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = year(); _o->year = _e; }
  { auto _e = species_population(); if (_e) { _o->species_population.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { if(_o->species_population[_i]) { _e->Get(_i)->UnPackTo(_o->species_population[_i].get(), _resolver); } else { _o->species_population[_i] = std::unique_ptr<Ecosystem::SpeciesPopulationT>(_e->Get(_i)->UnPack(_resolver)); }; } } else { _o->species_population.resize(0); } }
}

inline flatbuffers::Offset<WorldPopulation> WorldPopulation::Pack(flatbuffers::FlatBufferBuilder &_fbb, const WorldPopulationT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateWorldPopulation(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<WorldPopulation> CreateWorldPopulation(flatbuffers::FlatBufferBuilder &_fbb, const WorldPopulationT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const WorldPopulationT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _year = _o->year;
  auto _species_population = _o->species_population.size() ? _fbb.CreateVector<flatbuffers::Offset<Ecosystem::SpeciesPopulation>> (_o->species_population.size(), [](size_t i, _VectorArgs *__va) { return CreateSpeciesPopulation(*__va->__fbb, __va->__o->species_population[i].get(), __va->__rehasher); }, &_va ) : 0;
  return Ecosystem::CreateWorldPopulation(
      _fbb,
      _year,
      _species_population);
}

inline const flatbuffers::TypeTable *RawPopulationTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_UINT, 0, -1 }
  };
  static const int64_t values[] = { 0, 4, 8 };
  static const char * const names[] = {
    "male_population",
    "female_population"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_STRUCT, 2, type_codes, nullptr, nullptr, values, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *SpeciesPopulationTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    Ecosystem::RawPopulationTypeTable
  };
  static const char * const names[] = {
    "kind",
    "matable_population",
    "non_matable_population"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *WorldPopulationTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 1, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    Ecosystem::SpeciesPopulationTypeTable
  };
  static const char * const names[] = {
    "year",
    "species_population"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 2, type_codes, type_refs, nullptr, nullptr, names
  };
  return &tt;
}

inline const Ecosystem::WorldPopulation *GetWorldPopulation(const void *buf) {
  return flatbuffers::GetRoot<Ecosystem::WorldPopulation>(buf);
}

inline const Ecosystem::WorldPopulation *GetSizePrefixedWorldPopulation(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<Ecosystem::WorldPopulation>(buf);
}

inline WorldPopulation *GetMutableWorldPopulation(void *buf) {
  return flatbuffers::GetMutableRoot<WorldPopulation>(buf);
}

inline Ecosystem::WorldPopulation *GetMutableSizePrefixedWorldPopulation(void *buf) {
  return flatbuffers::GetMutableSizePrefixedRoot<Ecosystem::WorldPopulation>(buf);
}

inline bool VerifyWorldPopulationBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Ecosystem::WorldPopulation>(nullptr);
}

inline bool VerifySizePrefixedWorldPopulationBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Ecosystem::WorldPopulation>(nullptr);
}

inline void FinishWorldPopulationBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Ecosystem::WorldPopulation> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedWorldPopulationBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Ecosystem::WorldPopulation> root) {
  fbb.FinishSizePrefixed(root);
}

inline std::unique_ptr<Ecosystem::WorldPopulationT> UnPackWorldPopulation(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<Ecosystem::WorldPopulationT>(GetWorldPopulation(buf)->UnPack(res));
}

inline std::unique_ptr<Ecosystem::WorldPopulationT> UnPackSizePrefixedWorldPopulation(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<Ecosystem::WorldPopulationT>(GetSizePrefixedWorldPopulation(buf)->UnPack(res));
}

}  // namespace Ecosystem

#endif  // FLATBUFFERS_GENERATED_POPULATION_ECOSYSTEM_H_
