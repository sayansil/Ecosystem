#include <god.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include <stat_fetcher.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

struct pyecosystem
{
    std::unique_ptr<God> guru_nanak;

    pyecosystem(const bool &gods_eye = false)
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

    void set_disable_deaths(const bool &val)
    {
        guru_nanak->disable_deaths = val;
    }

    void spawn_organism(const std::string &full_species_name, const int &age, const bool &monitor, const std::string &name)
    {
        const std::string &kingdom = full_species_name.substr(0, full_species_name.find('/'));
        const std::string &species = full_species_name.substr(full_species_name.find('/') + 1);

        if (kingdom == "animal")
            guru_nanak->spawn_organism(std::make_shared<Animal>(species, age, monitor, name));
        else if (kingdom == "plant")
            guru_nanak->spawn_organism(std::make_shared<Plant>(species, age, monitor, name));
    }

    std::unordered_map<std::string, std::vector<double>> test_organism(const std::string &full_species_name, const int &age, const std::vector<std::string> &vars, const int &years)
    {
        std::unordered_map<std::string, std::vector<double>> res;
        const std::string &kingdom = full_species_name.substr(0, full_species_name.find('/'));
        const std::string &species = full_species_name.substr(full_species_name.find('/') + 1);

        if (kingdom == "animal")
            res = guru_nanak->test_organism(std::make_shared<Animal>(species, age), vars, years);
        else if (kingdom == "plant")
            res = guru_nanak->test_organism(std::make_shared<Plant>(species, age), vars, years);

        return res;
    }

    void happy_new_year(const bool &log)
    {
        guru_nanak->happy_new_year(log);
    }

    void remember_species(const std::string& full_species_name)
    {
        guru_nanak->remember_species(full_species_name);
    }

    std::string get_annual_data(const std::string& full_species_name)
    {
        return guru_nanak->get_annual_data(full_species_name);
    }

    std::vector<std::map<std::string, std::string>> get_live_data()
    {
        return guru_nanak->get_live_data();
    }

    bool load_snapshot_from_year(int year) {
        if (guru_nanak) {
            return guru_nanak->load_snapshot(year);
        }
        // Consider logging an error if guru_nanak is null
        return false;
    }

    std::vector<int> get_list_of_available_snapshots() {
        if (guru_nanak && guru_nanak->db) {
            return guru_nanak->db->get_available_years();
        }
        // Consider logging an error if guru_nanak or db is null
        return {};
    }

    int get_current_year() {
        if (guru_nanak) {
            return guru_nanak->year; // Accessing public member 'year' of God class
        }
        // Consider logging an error or throwing an exception if guru_nanak is null
        return 0;
    }
};

PYBIND11_MODULE(pyecosystem, m)
{
    pybind11::class_<pyecosystem>(m, "pyecosystem")
        .def(pybind11::init<const bool &>())
        .def("reset_species", &pyecosystem::reset_species)
        .def("set_monitor_offsprings", &pyecosystem::set_monitor_offsprings)
        .def("set_disable_deaths", &pyecosystem::set_disable_deaths)
        .def("spawn_organism", &pyecosystem::spawn_organism)
        .def("test_organism", &pyecosystem::test_organism)
        .def("happy_new_year", &pyecosystem::happy_new_year)
        .def("remember_species", &pyecosystem::remember_species)
        .def("get_annual_data", &pyecosystem::get_annual_data)
        .def("get_live_data", &pyecosystem::get_live_data)
        .def("load_snapshot_from_year", &pyecosystem::load_snapshot_from_year, "Loads a simulation snapshot from a specific year.")
        .def("get_list_of_available_snapshots", &pyecosystem::get_list_of_available_snapshots, "Returns a list of years for which snapshots are available.")
        .def("get_current_year", &pyecosystem::get_current_year, "Gets the current simulation year.");
}
