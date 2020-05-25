#include <algorithm>
#include <array>
#include <bits/c++config.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string.h> //memchr
#include <sys/socket.h>
#include <netdb.h>	//hostent
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <unordered_map>
#include <atomic>

#include "../include/config.hpp"
#include "../include/server.hpp"

extern char **environ;

const std::size_t BUF_SIZE = 1024;
const std::int32_t PIPE_READ_FD = 0;
const std::int32_t PIPE_WRIT_FD = 1;

// Reads data from the socket descriptor
std::string read_from_sd(const std::uint64_t);

// Executes the process and 
// reads/ writes from the passed socket descriptor 
// to the process std file descriptor.
std::int64_t exec(const torc::cfg::Proc&, std::uint64_t);

// write string to a socket descriptor
void write_to_sd(const std::string, std::uint64_t);


torc::svc::exitcode torc::svc::start(const torc::cfg::Base cfg)
{
    torc::svc::atomic_umap_t proc_th_cnt;
    for (auto& it: cfg.b_procs)
    {
        proc_th_cnt[it.first] = torc::svc::atomic_ptr_t(new std::atomic_uint32_t(it.second.p_t_cnt));
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

std::string read_from_sd(const std::uint64_t sd)
{
    std::size_t bytes_read = 0;
    char buff[BUF_SIZE];    

    std::string result;

    while ((bytes_read = recv(sd, buff, BUF_SIZE, 0)) > 0)
    {
        std::string input;
        char* z_byt = static_cast<char*>(memchr(buff, '\0', bytes_read));
        z_byt = z_byt ? z_byt : static_cast<char*>(memchr(buff, '\r', bytes_read));
        z_byt = z_byt ? z_byt : static_cast<char*>(memchr(buff, '\n', bytes_read));
        input.assign(buff, z_byt ? z_byt - buff : bytes_read);

        result += input;

        if (bytes_read < BUF_SIZE) break;
    } 
    
    return result;
}

void torc::svc::connection_handler
(
    const std::uint64_t sock_desc, 
    const torc::cfg::Base& cfg, 
    const torc::svc::atomic_umap_t& proc_th_cnt
)
{
    std::atomic_uint32_t* count;

    auto input = read_from_sd(sock_desc);

    try 
    {
        auto proc = cfg.b_procs.at(input);

        count = proc_th_cnt.at(input).get();

        (*count)--;

        input = read_from_sd(sock_desc);

        if (input == "invoke")
        {
            write(sock_desc, proc.p_cmd.c_str(), proc.p_cmd.length());
            exec(proc, sock_desc);
        }
    } catch (const std::out_of_range& oorex) 
    {
        write(sock_desc, "torcd: Not found!", 17);
    }
    

    if (count)
    {
        (*count)++;
    }
    
    close(sock_desc);
}

std::int64_t exec(const torc::cfg::Proc& proc, std::uint64_t sd)
{
    std::array<std::int32_t, 2> stdin_pipes;
    std::array<std::int32_t, 2> stdout_pipes;

    std::int64_t result, child;

    if (pipe(stdin_pipes.data()) < 0)
    {
        write_to_sd("torcd: error allocating read pipe for child proc", sd);
        
        return -1;
    }

    if (pipe(stdout_pipes.data()) < 0)
    {
        close(stdin_pipes[PIPE_READ_FD]);
        close(stdin_pipes[PIPE_WRIT_FD]);

        write_to_sd("torcd: error allocating write pipe for child proc", sd);
        
        return -1;
    }

    child = vfork();
    if (child == 0) // child
    {
        if (dup2(stdin_pipes[PIPE_READ_FD], STDIN_FILENO) == -1) // redirect stdin
        {
            write_to_sd("torcd: error redirecting stdin for child proc", sd);

            exit(errno);
        }

        if (dup2(stdout_pipes[PIPE_WRIT_FD], STDOUT_FILENO) == -1) // redirect stdout
        {
            write_to_sd("torcd: error redirecting stdout for child proc", sd);

            exit(errno);
        }        

        if (dup2(stdout_pipes[PIPE_WRIT_FD], STDERR_FILENO) == -1) //redirect stderr
        {
            write_to_sd("torcd: error redirecting stderr for child proc", sd);

            exit(errno);
        }        

        close(stdin_pipes[PIPE_READ_FD]);
        close(stdin_pipes[PIPE_WRIT_FD]);
        close(stdout_pipes[PIPE_READ_FD]);
        close(stdout_pipes[PIPE_WRIT_FD]);

        char** argv = {};

        result = execve(proc.p_cmd.c_str(), argv, environ);

        exit(result);
    }
    else if (child > 0)
    {
        close(stdin_pipes[PIPE_READ_FD]);
        close(stdout_pipes[PIPE_WRIT_FD]);

        char buf[BUF_SIZE];
        std::size_t bytes_read;

        while ( (bytes_read = read(stdout_pipes[PIPE_READ_FD], buf, BUF_SIZE)) > 0)
        {
            // write back to the client sock
            write(sd, buf, bytes_read);
        }
        
        close(stdin_pipes[PIPE_WRIT_FD]);
        close(stdout_pipes[PIPE_READ_FD]);        
    }
    else
    {
        close(stdin_pipes[PIPE_READ_FD]);
        close(stdin_pipes[PIPE_WRIT_FD]);
        close(stdout_pipes[PIPE_READ_FD]);
        close(stdout_pipes[PIPE_WRIT_FD]);        
    }
}

void write_to_sd(const std::string msg, std::uint64_t sd)
{
    write(sd, msg.c_str(), msg.length());
}