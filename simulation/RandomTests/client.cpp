#include "tbb/task_group.h"
#include "zmq.hpp"
#include <iostream>
#include <string>
#include <tbb/tbb.h>
#include <chrono>

zmq::context_t context(1);
zmq::socket_t socket(context, zmq::socket_type::dealer);

unsigned int count = 1;

void send()
{
    std::string send_message = std::to_string(count) + ": Hello from client";
    std::cout << "Sending from client: " << send_message << '\n';
    socket.send(zmq::buffer(send_message));
    zmq::message_t reply;
    socket.recv(reply);
    std::cout << "Received: " << reply.to_string() << '\n';
    count++;
}
int main()
{
    tbb::task_group g;
    socket.connect("tcp://localhost:5555");
    std::chrono::steady_clock::time_point start;
    start = std::chrono::steady_clock::now();

    while(true)
    {
        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() >= 5)
        {
            g.run(send);
            start = std::chrono::steady_clock::now();
        }
    }
    g.wait();
}
