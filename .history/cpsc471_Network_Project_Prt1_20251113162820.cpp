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

int main(int argc, char* argv[]) 
{
    std::cout << "Hello, world! Testing if it compiles\n";

    int port = DEFAULT_PORT;
    std::cout << "<IP_ADDRESS> <PORT>\n"; // example: <127.0.0.1> <8080>

    if(argc != 3)
    {
        std::cerr << "How to use -> " << argv[0] << " <PROGRAM FILE> <IP_ADDRESS> <PORT>\n";
        return 1;
    }

    std::string ip_address = argv[1];    
    port = std::stoi(argv[2]);

    std::cout << "IP Address: " << ip_address << "\n";
    std::cout << "Port: " << port << "\n";

    return 0;
}