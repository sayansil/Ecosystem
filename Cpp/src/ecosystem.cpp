#include <god.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include <stat_fetcher.hpp>
#include <pybind11/pybind11.h>

struct pyecosystem
{
    std::unique_ptr<God> guru_nanak;

    pyecosystem(const bool &gods_eye)
    {
        guru_nanak = std::make_unique<God>(gods_eye);
    }

    void reset_species(const std::string& str)
    {
        guru_nanak->reset_species(str);
    }

    void set_monitor_offsprings(const bool& val)
    {
        guru_nanak->monitor_offsprings = val;
    }

    void spawn_plant(const std::string& species, const int& num, const bool& val)
    {
        guru_nanak->spawn_organism(std::make_shared<Plant>(species, num, val));
    }

    void spawn_animal(const std::string& species, const int& num, const bool& val)
    {
        guru_nanak->spawn_organism(std::make_shared<Animal>(species, num, val));
    }

    // void start_listening_for_simulation()
    // {
    //     guru_nanak->start_listening_for_simulation();
    // }

    void happy_new_year(const bool &val)
    {
        guru_nanak->happy_new_year(val);
    }

    void remember_species(const std::string& str)
    {
        guru_nanak->remember_species(str);
    }

    std::string get_annual_data(const std::string& full_species_name)
    {
        return guru_nanak->get_annual_data(full_species_name);
    }
};

PYBIND11_MODULE(pyecosystem, m)
{
    pybind11::class_<pyecosystem>(m, "pyecosystem")
        .def(pybind11::init<const bool&>())
        .def("reset_species", &pyecosystem::reset_species)
        .def("set_monitor_offsprings", &pyecosystem::set_monitor_offsprings)
        .def("spawn_plant", &pyecosystem::spawn_plant)
        .def("spawn_animal", &pyecosystem::spawn_animal)
        //.def("start_listening_for_simulation", &pyecosystem::start_listening_for_simulation)
        .def("happy_new_year", &pyecosystem::happy_new_year)
        .def("remember_species", &pyecosystem::remember_species)
        .def("get_annual_data", &pyecosystem::get_annual_data);
}
