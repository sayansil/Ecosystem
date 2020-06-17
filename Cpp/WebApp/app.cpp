#include <App.h>
#include <iostream>
#include <helper.hpp>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <argh.h>
#include <animal.hpp>
#include <plant.hpp>
#include <god.hpp>

const int port_number = 8080;
std::string message = "Hello " + std::to_string(port_number);


std::unordered_map<std::string, std::string> fetch_params(const std::string_view &query)
{
    auto replaceAll = [](std::string &source, const std::string &from, const std::string &to) {
        std::string newString;
        newString.reserve(source.length());

        std::string::size_type lastPos = 0;
        std::string::size_type findPos;

        while(std::string::npos != (findPos = source.find(from, lastPos)))
        {
            newString.append(source, lastPos, findPos - lastPos);
            newString += to;
            lastPos = findPos + from.length();
        }

        newString += source.substr(lastPos);
        source.swap(newString);
    };

    std::unordered_map<std::string, std::string> query_map;
    int i, index = 0;
    std::string key, value;
    for (i = 0; i < query.length(); i++)
    {
        if (query[i] == '=')
        {
            key = query.substr(index, i - index);
            index = i + 1;
        }
        else if (query[i] == '&')
        {
            value = query.substr(index, i - index);
            replaceAll(value, "%20", " ");
            replaceAll(value, "%22", "");
            index = i + 1;
            query_map[key] = value;
        }
    }

    value = query.substr(index, i - index);
    replaceAll(value, "%20", " ");
    replaceAll(value, "%22", "");
    index = i + 1;
    query_map[key] = value;

    return query_map;
}

bool authenticate_api(const std::string &key)
{
    std::vector<std::string> API_KEYS;

    std::ifstream json_file(helper::get_ecosystem_root() / "Cpp" / "WebApp" / "keys.json");
    nlohmann::json key_j;

    json_file >> key_j;

    API_KEYS = std::vector<std::string>(key_j["API-KEYS"]);

    bool auth = false;
    for (auto k : API_KEYS)
    {
        if (key == k)
        {
            auth = true;
            break;
        }
    }

    return auth;
}


int main(int argc, char** argv) {
    argh::parser cmdl(argc, argv);
    std::string ip_address = cmdl("ip").str();
    if(ip_address.length() == 0)
        ip_address = "0.0.0.0";

    bool available = true;

    uWS::App()
        .get("/", [](auto *res, auto *req) { res->end("Ok"); })
        .listen(ip_address, port_number, [&ip_address](auto *listenSocket) {
            if (listenSocket)
            {
                std::cout << "Listening at " << ip_address << ":" << port_number << ".\n";
            }
        })

        .get("/query", [&available](auto *res, auto *req) {
            auto query_map = fetch_params(req->getQuery());
            nlohmann::json response = {
                {"status", ""},
                {"log", ""},
                {"data", ""}
            };

            if (!available)
            {
                response["status"] = "2";
                response["log"] = "Server Busy";
            }
            else if (query_map.find("api-key") == query_map.end() || !authenticate_api(query_map["api-key"]))
            {
                response["status"] = "1";
                response["log"] = "Invalid API-KEY";
            }
            else
            {
                available = false;

                int initial_organism_count = query_map.find("initial_count") == query_map.end() ?
                    200 : std::stoi(query_map["initial_count"]);
                int years_to_simulate = query_map.find("years") == query_map.end() ?
                    100 : std::stoi(query_map["years"]);
                std::string kingdom = query_map.find("kingdom") == query_map.end() ?
                    "plant" : query_map["kingdom"];
                std::string species = query_map.find("species") == query_map.end() ?
                    "bamboo" : query_map["species"];

                std::string full_species_name = kingdom + "/" + species;

                God laxmi(false);
                laxmi.reset_species(full_species_name);

                while (initial_organism_count--)
                {
                    if (kingdom == "animal")
                        laxmi.spawn_organism(std::make_shared<Animal>(species, 10));
                    else if (kingdom == "plant")
                        laxmi.spawn_organism(std::make_shared<Plant>(species, 10));
                }

                while (years_to_simulate--)
                {
                    laxmi.happy_new_year(true);
                    laxmi.remember_species(full_species_name);
                }

                response["status"] = "0";
                response["log"] = "";
                response["data"] = laxmi.get_annual_data(full_species_name);

                available = true;
            }

            res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(response.dump());
        })

        .get("/help/menu", [](auto *res, auto *req) {
            nlohmann::json menu = {
                {"animal", {"deer", "lion"}},
                {"plant", {"bamboo"}}
            };

            res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(menu.dump());
        })

        .get("/help/schema", [](auto *res, auto *req) {
            nlohmann::json schema = {
                {"animal", {"YEAR", "MALE", "FEMALE", "M_MALE", "M_FEMALE", "C_PROB", "M_AGE_START", "M_AGE_END", "MX_AGE", "MT_PROB", "OF_FACTOR", "AGE_DTH", "FIT_DTH", "AFR_DTH", "HT_SP", "HT_ST", "HT_VT", "WT_SP", "WT_ST", "WT_VT", "VT_AP", "VT_SP", "ST_AP", "ST_SP", "TMB_AP", "TMB_HT", "TMB_SP", "TMB_ST", "TMB_VT", "TMB_WT", "TM_HT", "TM_SP", "TM_WT", "TMM_HT", "TMM_SP", "TMM_ST", "TMM_VT", "TMM_WT", "SL_FACTOR", "AVG_GEN", "AVG_IMM", "AVG_AGE", "AVG_HT", "AVG_WT", "AVGMA_AP", "AVGMA_SP", "AVGMA_ST", "AVGMA_VT", "AVG_SFIT", "AVG_DTHF", "AVG_VIS"}},
                {"plant", {"YEAR", "POP", "M_POP", "C_PROB", "M_AGE_START", "M_AGE_END", "MX_AGE", "MT_PROB", "OF_FACTOR", "AGE_DTH", "FIT_DTH", "AFR_DTH", "HT_VT", "WT_VT", "TMB_HT", "TMB_VT", "TMB_WT", "TM_HT", "TM_WT", "TMM_HT", "TMM_VT", "TMM_WT", "AVG_GEN", "AVG_IMM", "AVG_AGE", "AVG_HT", "AVG_WT", "AVGMA_VT", "AVG_SFIT", "AVG_DTHF"}}
            };

            res->writeHeader("Content-Type", "application/json; charset=utf-8")->end(schema.dump());
        })

        .get("/available", [available](auto *res, auto *req) {
            std::string response = available ? "true" : "false";
            res->end(response);
        })

        .run();
}
