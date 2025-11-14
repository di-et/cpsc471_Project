#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080


int main(int argc, char* argv[]) 
{
    std::cout << "Hello, world!\n";
    if(argc != 3) 
    {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT>\n";
        return 1;
    }

    
    return 0;
}