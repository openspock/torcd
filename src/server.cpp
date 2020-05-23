#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>	//hostent
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <unordered_map>
#include <atomic>

#include "../include/config.hpp"
#include "../include/server.hpp"

torc::svc::exitcode torc::svc::start(const torc::cfg::Base cfg)
{
    std::unordered_map<std::string, std::atomic_uint32_t> proc_th_cnt;
    for (auto& it: cfg.b_procs)
    {
        proc_th_cnt.emplace(it.first, it.second.p_t_cnt);
    }

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

    std::cout << "starting to accept incoming connections" << std::endl;

    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        if (new_socket < 0)
        {
            std::cerr << "Client accept failed" << std::endl;
            return exitcode::boot_failure;
        }

        std::cout << "Received ping from client with id: " << new_socket << std::endl;

        std::thread t(connection_handler, new_socket, std::ref(cfg), std::ref(proc_th_cnt));

        t.join();
    }

    return torc::svc::exitcode::graceful_shutdown;
}

void torc::svc::connection_handler
(
    const std::uint64_t sock_desc, 
    const torc::cfg::Base& cfg, 
    const std::unordered_map<std::string, std::atomic_uint32_t>& proc_th_cnt
)
{
    write(sock_desc, "Hello!\n", 8);
    close(sock_desc);
}   