#include "zmq.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>

zmq::context_t context(1);
zmq::socket_t socket(context, zmq::socket_type::dealer);
unsigned int count = 1;

void listen()
{
    zmq::message_t request;
    while(true)
    {
        socket.recv(request);
        std::string request_str = request.to_string();
        if(request_str.length() > 0)
        {
            std::cout << "Received " << request_str << '\n';
            std::string send_message = std::to_string(count) + ": Hello from server";
            std::cout << "Sending: " << send_message << '\n';
            socket.send(zmq::buffer(send_message));
            count++;
        }
    }
}

int main()
{
    socket.bind("tcp://*:5555");
    std::thread t(listen);
    t.join();
}
