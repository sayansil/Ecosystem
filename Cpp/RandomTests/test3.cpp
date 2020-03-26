#include "nlohmann/json_fwd.hpp"
#include <iomanip>
#include <ostream>
#include <string>
#include <zmq.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::dealer);
    socket.connect("tcp://localhost:5556");

    for(int i = 0; i < 200; i++)
    {
        std::cout << "i = " << i << '\n';
        zmq::message_t reply;
        socket.recv(reply);
        std::string str = reply.to_string();
        if(str.length() > 0)
        {
            std::cout << "Received:\n" << nlohmann::json(str) << '\n';
        }
    }

}
