#include "nlohmann/json_fwd.hpp"
#include <iomanip>
#include <ostream>
#include <string>
#include <zmq.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::dealer);
    socket.connect("tcp://localhost:5556");

    for(int i = 0; i < 10; i++)
    {
        std::cout << "i = " << i << '\n';
        socket.send(zmq::str_buffer("SEND"));
        zmq::message_t reply;
        socket.recv(reply);
        std::string str = reply.to_string();
        if(str.length() > 0)
        {
            std::cout << "Received ";
            nlohmann::json data = nlohmann::json::parse(str);
            std::cout << data["status"] << '\n';
        }
        //std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    socket.send(zmq::str_buffer("STOP"));
}
