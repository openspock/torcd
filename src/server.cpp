#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>	//hostent
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/config.hpp"
#include "../include/server.hpp"

using torc::svc::exitcode;
    
exitcode torc::svc::start(const torc::cfg::Base cfg)
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cerr << "Could not create socket" << std::endl;
        return exitcode::boot_failure;
    }
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( cfg.b_port );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        std::cerr << "Could not create socket" << std::endl;
        return exitcode::boot_failure;
    }            

    //Listen
    listen(socket_desc , 3);

    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        if (new_socket < 0)
        {
            std::cerr << "Client accept failed" << std::endl;
            return exitcode::boot_failure;
        }

        std::cout << "Received ping from client with id: " << new_socket << std::endl;

    }

    return exitcode::graceful_shutdown;
}