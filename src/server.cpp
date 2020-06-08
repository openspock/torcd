#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>//memchr
#include <sys/socket.h>
#include <sys/signal.h>
#include <netdb.h>//hostent
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <unordered_map>
#include <atomic>
#include <vector>

#include "../include/config.hpp"
#include "../include/spdlog.h"
#include "../include/server.hpp"


const std::int32_t BUF_SIZE = 1024;
const std::int32_t PIPE_READ_FD = 0;
const std::int32_t PIPE_WRIT_FD = 1;

// Executes the process and
// reads/ writes from the passed socket descriptor
// to the process std file descriptor.
std::int64_t exec(const torc::cfg::Proc &, std::int32_t);

// read buffer char data from the socket descriptor
// and return a string.
std::string read_from_sd(const std::int32_t);

// write string to a socket descriptor
void write_to_sd(const char *, std::int32_t);

torc::svc::exitcode torc::svc::start(const torc::cfg::Base cfg)
{
  auto console = spdlog::stdout_color_mt("console");

  torc::svc::atomic_umap_t proc_th_cnt;
  for (const auto &it : cfg.b_procs) {
    proc_th_cnt[it.first] = std::make_shared<std::atomic_uint32_t>(it.second.p_t_cnt);
  }

  int socket_desc = 0;
  int new_socket = 0;
  int c = 0;
  struct sockaddr_in server
  {
  };
  struct sockaddr_in client
  {
  };

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    std::cerr << "Could not create socket" << std::endl;
    return exitcode::boot_failure;
  }

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(cfg.b_port);

  //Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    std::cerr << "Could not create socket" << std::endl;
    return exitcode::boot_failure;
  }

  //Listen
  listen(socket_desc, 3);

  c = sizeof(struct sockaddr_in);
  while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
    if (new_socket < 0) {
      std::cerr << "Client accept failed" << std::endl;
      return exitcode::boot_failure;
    }

    char *client_ip = inet_ntoa(client.sin_addr);
    int client_port = ntohs(client.sin_port);

    console->info("host: {}, port: {}", client_ip, client_port);

    std::thread t(connection_handler, new_socket, std::ref(cfg), std::ref(proc_th_cnt));

    t.detach();
  }

  return torc::svc::exitcode::graceful_shutdown;
}

std::string read_from_sd(const std::int32_t sd)
{
  std::int32_t bytes_read = 0;
  std::array<char, BUF_SIZE> buff;

  std::string result;

  while ((bytes_read = read(sd, buff.data(), BUF_SIZE)) > 0) {
    std::string input;
    char *z_byt = static_cast<char *>(memchr(buff.data(), '\0', bytes_read));
    z_byt = z_byt != nullptr ? z_byt : static_cast<char *>(memchr(buff.data(), '\r', bytes_read));
    z_byt = z_byt != nullptr ? z_byt : static_cast<char *>(memchr(buff.data(), '\n', bytes_read));
    input.assign(buff.data(), z_byt != nullptr ? z_byt - buff.data() : bytes_read);

    result += input;

    if (bytes_read < BUF_SIZE) {
      break;
    }
  }

  return result;
}

void torc::svc::connection_handler(const std::int32_t sock_desc, const torc::cfg::Base &cfg, const torc::svc::atomic_umap_t &proc_th_cnt)
{
  auto input = read_from_sd(sock_desc);

  try {
    auto proc = cfg.b_procs.at(input);

    std::atomic_uint32_t *count = proc_th_cnt.at(input).get();

    if (*count == 0) {
      write_to_sd("torcd: busy", sock_desc);
    } else {
      input = read_from_sd(sock_desc);

      if (input == "invoke") {
        (*count)--;
        exec(proc, sock_desc);
        (*count)++;
      }
    }
  } catch (const std::out_of_range &oorex) {
    std::stringstream msg;
    msg << "torcd: " << input << ":Not found!" << std::endl;
    write_to_sd(msg.str().c_str(), sock_desc);
  }

  close(sock_desc);
}

std::int64_t exec(const torc::cfg::Proc &proc, std::int32_t sd)
{
  std::array<std::int32_t, 2> stdin_pipes;
  std::array<std::int32_t, 2> stdout_pipes;

  std::int64_t result, child;

  if (pipe(stdin_pipes.data()) < 0) {
    write_to_sd("torcd: error allocating read pipe for child proc", sd);

    return -1;
  }

  if (pipe(stdout_pipes.data()) < 0) {
    close(stdin_pipes[PIPE_READ_FD]);
    close(stdin_pipes[PIPE_WRIT_FD]);

    write_to_sd("torcd: error allocating write pipe for child proc", sd);

    return -1;
  }

  child = fork();
  if (child == 0)// child
  {
    if (dup2(stdin_pipes[PIPE_READ_FD], STDIN_FILENO) == -1)// redirect stdin
    {
      write_to_sd("torcd: error redirecting stdin for child proc", sd);

      exit(errno);
    }

    if (dup2(stdout_pipes[PIPE_WRIT_FD], STDOUT_FILENO) == -1)// redirect stdout
    {
      write_to_sd("torcd: error redirecting stdout for child proc", sd);

      exit(errno);
    }

    if (dup2(stdout_pipes[PIPE_WRIT_FD], STDERR_FILENO) == -1)//redirect stderr
    {
      write_to_sd("torcd: error redirecting stderr for child proc", sd);

      exit(errno);
    }

    close(stdin_pipes[PIPE_READ_FD]);
    close(stdin_pipes[PIPE_WRIT_FD]);
    close(stdout_pipes[PIPE_READ_FD]);
    close(stdout_pipes[PIPE_WRIT_FD]);

    char name[proc.p_name.length() + 1];
    strcpy(name, proc.p_name.c_str());
    char *argv[] = { name, NULL };

    result = execve(proc.p_cmd.c_str(), argv, environ);

    exit(result);
  } else if (child > 0) {
    close(stdin_pipes[PIPE_READ_FD]);
    close(stdout_pipes[PIPE_WRIT_FD]);

    char buf[BUF_SIZE];
    std::int32_t bytes_read;

    while ((bytes_read = read(stdout_pipes[PIPE_READ_FD], buf, BUF_SIZE)) > 0) {
      // write back to the client sock
      write(sd, buf, bytes_read);
    }

    close(stdin_pipes[PIPE_WRIT_FD]);
    close(stdout_pipes[PIPE_READ_FD]);
  } else {
    close(stdin_pipes[PIPE_READ_FD]);
    close(stdin_pipes[PIPE_WRIT_FD]);
    close(stdout_pipes[PIPE_READ_FD]);
    close(stdout_pipes[PIPE_WRIT_FD]);
  }

  return child;
}

void write_to_sd(const char *msg, std::int32_t sd)
{
  write(sd, msg, strlen(msg));
}
