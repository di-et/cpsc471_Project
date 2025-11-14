#include <iostream>
#include <cstring>
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
    struct sockaddr_in address;

    if (server_fd == -1)
    {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    address.sin_port = htons(port);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
    }

    // listen for connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
    }
    std::cout << "Server is listening on " << ip_address 
    << "at port " << port << std::endl;

    socklen_t addrlen = sizeof(address);
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