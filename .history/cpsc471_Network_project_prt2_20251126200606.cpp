#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <thread>

using namespace std;

#define DEFAULT_PORT 8080

// function that will start the server call it from main using the ip_address and port I just parsed

void startServer(const std::string &ip_address, int port);
void handleLsCmmd(int client_fd);
void handleGetCmmd(int client_fd, const std::string &filename);
void handlePutCmmd(int client_fd, std::string &filename);
void handleClientSession(int client_fd);


void handleClientSession(int client_fd)
{
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
            handleGetCmmd(client_fd, command.substr(4));
            continue;
        }

        if (command.rfind("PUT", 0) == 0)
        {
            std::string filename = command.substr(4);
            handlePutCmmd(client_fd, filename);
            continue;
        }

        const char* invalid = "Invalid command.\n";
        send(client_fd, invalid, strlen(invalid), 0);        
    }

    close(client_fd);
}

void handleLsCmmd(int client_fd)
{
    DIR *directory_pointer = opendir("."); // Open current directory
    if (directory_pointer == nullptr)
    {
        const char *err_msg = "Failed to open directory.\n";
        send(client_fd, err_msg, strlen(err_msg), 0);
        return;
    }

    std::string directory_listing;
    struct dirent* directory_entry;

    while ((directory_entry = readdir(directory_pointer)) != nullptr)
    {
        std:: string entry_name = directory_entry -> d_name;

        if(entry_name == "." || entry_name == "..")
        {
            continue; // Skip current and parent directory entries
        }

        directory_listing += entry_name + "\n";
        directory_listing += "\n";
    }

    closedir(directory_pointer);
    
    if (directory_listing.empty())
    {
        directory_listing = "Directory is empty.\n";
    }

    send(client_fd, directory_listing.c_str(), directory_listing.length(), 0);
}

void handleGetCmmd(int client_fd, const std::string &filename)
{
    std::ifstream file_stream(filename, std::ios::binary);
    if (!file_stream.is_open())
    {
        std::string error_message = "Error: File not found: " + filename + "\n";
        send(client_fd, error_message.c_str(), error_message.length(), 0);
        return;
    }

    std::string file_contents((std::istreambuf_iterator<char>(file_stream)),
                               std::istreambuf_iterator<char>());
    
    file_stream.close();

    if(file_contents.empty())
    {
        std::string empty_message = "File is empty: " + filename + "\n";
        send(client_fd, empty_message.c_str(), empty_message.length(), 0);
        return;
    }

std::string header = "BEGINNING OF FILE\n";
send(client_fd, header.c_str(), header.length(), 0);

send(client_fd, file_contents.c_str(), file_contents.length(), 0);

std::string footer = "\nEND OF FILE\n";
send(client_fd, footer.c_str(), footer.length(), 0);
}

void handlePutCmmd(int client_fd, std::string &filename)
{
    const char *ready_msg = "READY TO RECEIVE FILE\n";
    send(client_fd, ready_msg, strlen(ready_msg), 0);

    std::ofstream output_file_stream(filename, std::ios::binary);
    if (!output_file_stream.is_open())
    {
        std::string error_message = "Error: Cannot create file: " + filename + "\n";
        send(client_fd, error_message.c_str(), error_message.length(), 0);
        return;
    }

    char receive_buffer[1024];
    bool done = false;

    while (!done)
    {
        ssize_t bytes_received = read(client_fd, receive_buffer, sizeof(receive_buffer) - 1);
        if (bytes_received <= 0)
        {
            done = true;
            break;
        }

        receive_buffer[bytes_received] = '\0';
        std::string data_chunk(receive_buffer);

        std::size_t eof_pos = data_chunk.find("EOF\n");
        if (eof_pos != std::string::npos)
        {
            output_file_stream.write(data_chunk.c_str(), eof_pos);
            done = true;
        }
        else
        {
            output_file_stream.write(data_chunk.c_str(), bytes_received);
        }
    }
    output_file_stream.close();

    std::string success_message = "File received successfully: " + filename + "\n";
    send(client_fd, success_message.c_str(), success_message.length(), 0);
}


void startServer(const std::string &ip_address, int port)
{
    std::cout << "Starting server on IP: " << ip_address << " Port: " << port << std::endl;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    struct sockaddr_in address;
    memset(&address, '0', sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    address.sin_port = htons(port);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return;
    }

    // listen for connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "Server listening on " << ip_address << " at port " << port << std::endl;
    socklen_t addrlen = sizeof(address);

    while(true)
    {
        int client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);

        if (client_fd < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        std::thread client_thread(handleClientSession, client_fd);
        client_thread.detach();
    }

    close(server_fd);
    std::cout << "Server stopped" << std::endl;

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