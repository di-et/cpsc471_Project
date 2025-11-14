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

void handle_Connection(int *client_socket) 
{
    int sock_fd = *client_socket;           // Dereference the pointer to get the socket file descriptor
    free(client_socket);                    // Free the allocated memory for client_socket

    
    



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