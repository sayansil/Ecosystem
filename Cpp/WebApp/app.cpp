#include <App.h>
#include <iostream>
#include <helper.hpp>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <nlohmann/json.hpp>

const int port_number = 8080;
std::string message = "Hello " + std::to_string(port_number);


std::unordered_map<std::string, std::string> fetch_params(const std::string_view &query)
{
    auto replaceAll = [](std::string &str, const std::string &from, const std::string &to) {
        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
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


int main() {
    auto fetchParams = &fetch_params;
    auto authenticateAPI = &authenticate_api;

    uWS::App()
        .get("/", [](auto *res, auto *req) { res->end("Ok"); })
        .listen("0.0.0.0", port_number, [](auto *listenSocket) {
            if (listenSocket)
            {
                std::cout << "Listening at " << port_number << "...\n";
            }
        })

        .get("/query", [fetchParams, authenticateAPI](auto *res, auto *req) {
            auto query_map = fetch_params(req->getQuery());

            if (authenticate_api(query_map["api-key"]))
            {
                std::cout << "Query map:\n";
                for (const auto &[x, y] : query_map)
                    std::cout << x << '=' << y << '\n';
                res->end("/query call recieved\n");
            }
            else
            {
                res->end("/query call recieved\n");
            }
        })

        .get("/help/menu", [](auto *res, auto *req) {
            nlohmann::json menu = {
                {"animal", {"deer", "lion"}},
                {"plant", {"bamboo"}}
            };

            res->end(menu.dump());
        })

        .get("/help/schema", [](auto *res, auto *req) {
            nlohmann::json schema = {
                {"animal", {"YEAR", "MALE", "FEMALE", "M_MALE", "M_FEMALE", "C_PROB", "M_AGE_START", "M_AGE_END", "MX_AGE", "MT_PROB", "OF_FACTOR", "AGE_DTH", "FIT_DTH", "AFR_DTH", "HT_SP", "HT_ST", "HT_VT", "WT_SP", "WT_ST", "WT_VT", "VT_AP", "VT_SP", "ST_AP", "ST_SP", "TMB_AP", "TMB_HT", "TMB_SP", "TMB_ST", "TMB_VT", "TMB_WT", "TM_HT", "TM_SP", "TM_WT", "TMM_HT", "TMM_SP", "TMM_ST", "TMM_VT", "TMM_WT", "SL_FACTOR", "AVG_GEN", "AVG_IMM", "AVG_AGE", "AVG_HT", "AVG_WT", "AVGMA_AP", "AVGMA_SP", "AVGMA_ST", "AVGMA_VT", "AVG_SFIT", "AVG_DTHF", "AVG_VIS"}},
                {"plant", {"YEAR", "POP", "M_POP", "C_PROB", "M_AGE_START", "M_AGE_END", "MX_AGE", "MT_PROB", "OF_FACTOR", "AGE_DTH", "FIT_DTH", "AFR_DTH", "HT_VT", "WT_VT", "TMB_HT", "TMB_VT", "TMB_WT", "TM_HT", "TM_WT", "TMM_HT", "TMM_VT", "TMM_WT", "AVG_GEN", "AVG_IMM", "AVG_AGE", "AVG_HT", "AVG_WT", "AVGMA_VT", "AVG_SFIT", "AVG_DTHF"}}
            };

            res->end(schema.dump());
        })

        .run();
}
