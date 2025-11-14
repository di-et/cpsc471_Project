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

void handle_Connection(int *sock_fd_ptr) 
{
    int sock_fd = *sock_fd_ptr;
    free(sock_fd_ptr);
    std::cout << "Handling connection on socket: " << sock_fd << std::endl;
    bool done = false;

    while (!done)
    {
        char buffer[1024] = {0};
        int bytes_read = read(sock_fd, buffer, sizeof(buffer));
        if (bytes_read == -1)
        {
            std::cerr << "Error reading from socket\n";
            done = true;
        }
        else if (bytes_read == 0)
        {
            std::cout << "Client disconnected\n";
            done = true;
        }
        else
        {
            std::cout << "Received " << bytes_read << " bytes: " << std::string(buffer, bytes_read) << std::endl;
            // Echo back the data
            int bytes_sent = send(sock_fd, buffer, bytes_read, 0);
            if (bytes_sent < 0)
            {
                std::cerr << "Error sending to socket\n";
                done = true;
            }
        }
    }
    
    std::cout << "Closing socket: " << sock_fd << std::endl;
    close(sock_fd);


}

int main(int argc, char* argv[]) 
{
    std::cout << "Hello, world! Testing if it compiles\n";

    int port = DEFAULT_PORT;
    std::cout << "<IP_ADDRESS> <PORT>\n";

    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT>\n";
        return 1;
    }

    std::string ip_address = argv[1];    
    port = std::stoi(argv[2]);

    std::cout << "IP Address: " << ip_address << "\n";
    std::cout << "Port: " << port << "\n";

    return 0;
}