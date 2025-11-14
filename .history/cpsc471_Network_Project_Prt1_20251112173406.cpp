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

void start_Server(int port) 
{
    // Placeholder for server start logic
    std::cout << "Starting server on port " << port << "...\n";

    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;

    int opt = 1;
    // socketlen_t addrlen = sizeof(address);
    socklen_t addrlen = sizeof(address);


}

int main(int argc, char* argv[]) 
{
    std::cout << "Hello, world! Testing if it compiles\n";

    int port = DEFAULT_PORT;

    if(argc != 3)                                                       // This if statement checks for correct number of arguments
    {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT>\n";
        return 1;
    }
    std::string ip_address = argv[1];                                   // Store the IP address from command line argument
    port = std::stoi(argv[2]);                                          // Convert the port number from string to integer

    std::cout << "IP Address: " << ip_address << "\n";
    std::cout << "Port: " << port << "\n";

    return 0;
}