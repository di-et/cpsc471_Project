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

void startServer(const std::string &ip_address, int port);
void handleLsCmmd(int client_fd);
void handlePutCmmd(int client_fd);
void handleGetCmmd(int client_fd);

void handleLsCmmd(int client_fd)
{
    const char *response = "Executing ls command...\n";
    send(client_fd, response, strlen(response), 0);
    // Here you would add the actual implementation to execute 'ls' and send the output back

    
}

void handlePutCmmd(int client_fd)
{
    const char *response = "Ready to receive file...\n";
    send(client_fd, response, strlen(response), 0);
    // Here you would add the actual implementation to receive a file from the client
}

void handleGetCmmd(int client_fd)
{
    const char *response = "Preparing to send file...\n";
    send(client_fd, response, strlen(response), 0);
    // Here you would add the actual implementation to send a file to the client
}

void startServer(const std::string &ip_address, int port)
{
    std::cout << "Starting server on IP: " << ip_address << " Port: " << port << std::endl;

    int server_fd;
    int client_fd;

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

    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);

    if (client_fd < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        close(server_fd);
    }

    std::cout << "Connection accepted" << std::endl;

    bool connection_active = true;
    char buffer[1024];

    while (connection_active)
    {
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);

        if (bytes_received <= 0)
        {
            std::cout << "client disconnected" << std::endl;
            break;
        }

        buffer[bytes_received] = '\0'; // null-terminate
        std::cout << "Received from client: " << buffer << std::endl;

        std::string command(buffer);
        if (!command.empty() && command.back() == '\n')
        {
            command.pop_back();
        }

        if (command == "EXIT")
        {
            std::cout << "Client requested exit" << std::endl;
            connection_active = false;
            break;
        }

        if (command == "ls")
        {
            handleLsCmmd(client_fd);
            continue;
        }

        if (command.rfind("GET", 0) == 0)
        {
            handleGetCmmd(client_fd);
            continue;
        }

        if (command.rfind("PUT", 0) == 0)
        {
            handlePutCmmd(client_fd);
            continue;
        }

        const char* invalid = "Invalid command.\n";
        send(client_fd, invalid, strlen(invalid), 0);        
    }

    close(client_fd);
    close(server_fd);

}

int main(int argc, char *argv[])
{
    std::cout << "Hello, world! Testing if it compiles\n";

    int port = DEFAULT_PORT;

    if (argc != 3)
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