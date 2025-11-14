#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define DEFAULT_PORT 8080

// function that will start the server call it from main using the ip_address and port I just parsed

void startServer(const std::string& ip_address, int port);


void startServer(const std::string& ip_address, int port)
{
    std::cout << "Starting server on IP: " << ip_address << " Port: " << port << std::endl;

    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }


}

int main(int argc, char* argv[]) 
{
    std::cout << "Hello, world! Testing if it compiles\n";

    int port = DEFAULT_PORT;

    if(argc != 3)
    {
        std::cerr << "How to use -> " << argv[0] << " <PROGRAM FILE> <IP_ADDRESS> <PORT>\n";
        return 1;
    }

    std::string ip_address = argv[1];    
    port = std::stoi(argv[2]);

    std::cout << "IP Address: " << ip_address << "\n";
    std::cout << "Port: " << port << "\n";

    startServer(ip_address, port);

    return 0;
}