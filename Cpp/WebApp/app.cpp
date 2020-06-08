#include <App.h>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

const int port_number = 8080;
std::string message = "Hello " + std::to_string(port_number);

int main() {
    uWS::App()
        .get("/hello", [](auto *res, auto *req) { res->end(message); })
        .listen("0.0.0.0", port_number, [](auto *listenSocket) {
                if (listenSocket)
                {
                    std::cout << "Listening at " << port_number << "...\n";
                }
        })
        .get("/query", [](auto *res, auto *req) {
                res->end("query called\n");
                std::string_view query = req->getQuery();
                std::unordered_map<std::string_view, std::string_view> query_map;
                int i, index = 0;
                std::string_view key, value;
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
                        index = i + 1;
                        query_map[key] = value;
                    }
                }

                value = query.substr(index, i - index);
                index = i + 1;
                query_map[key] = value;

                std::cout << "Query map:\n";
                for (const auto &[x, y] : query_map)
                    std::cout << x << '=' << y << '\n';
        })
        .run();
}
