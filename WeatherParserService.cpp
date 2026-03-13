//
// Created by Zdmor on 3/12/2026.
//

#include <iostream>
#include <zmq.hpp>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
    zmq::context_t zmqContext(1);
    zmq::socket_t socket(zmqContext, ZMQ_REP);
    socket.bind("tcp://localhost:5003");
    std::cout << "Listening on port 5003" << std::endl;
    std::string responseMessage;

    while (true) {
        try {
            zmq::message_t request;
            zmq::recv_result_t result = socket.recv(request);
            std::string requestString = request.to_string();
            size_t spacePosition = requestString.find(' ');

            if (spacePosition != std::string::npos) {
                std::string fileName = requestString.substr(0, spacePosition);
                std::string command = requestString.substr(spacePosition + 1);
                std::ifstream file(fileName);
                if (file.is_open()) {
                    json weatherData;
                    file >> weatherData;

                    if (command == "getTemp") {
                        double temp = weatherData["main"]["temp"];
                        responseMessage = std::to_string(temp);
                    }
                    else if (command == "getWeather") {
                        std::string weather = weatherData["weather"][0]["description"];
                        responseMessage = weather;
                    }

                }
                else {
                    responseMessage = "Error: Failed to open file";
                }
            }
            else {
                responseMessage = "Error: Invalid command";
            }
            zmq::message_t reply(responseMessage.begin(), responseMessage.end());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const std::exception& e) {
            responseMessage = "Error";
            zmq::message_t reply(responseMessage.begin(), responseMessage.end());
            socket.send(reply, zmq::send_flags::none);
        }
    }
}
