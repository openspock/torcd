#include <cstdint>
#include <iostream>
#include <optional>
#include <fcntl.h>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/libconfig.hpp"
#include "../include/config.hpp"
#include "../include/server.hpp"

constexpr int file_perm = 027;

void daemonize()
{
  pid_t pid = 0;

  if (getppid() == 1) {
    return; /* already a daemon */
  }
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE); /* fork error */
  }
  if (pid > 0) {
    exit(EXIT_SUCCESS); /* parent exits */
  }

  /* child (daemon) continues */
  int sid = setsid(); /* obtain a new process group */
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  int i = 0;
  for (i = getdtablesize(); i >= 0; --i) {
    close(i); /* close all descriptors */
  }

  i = open("/dev/null", O_RDWR | O_CLOEXEC);
  fcntl(i, F_DUPFD_CLOEXEC);
  fcntl(i, F_DUPFD_CLOEXEC); /* handle standart I/O */

  umask(file_perm); /* set newly created file permissions */

  chdir("/"); /* change running directory */

  signal(SIGCHLD, SIG_IGN); /* ignore child */
  signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
}

int main(int /*argc*/, char ** /*argv*/)
{
  try {
    auto config = torc::cfg::create().value();

    daemonize();

    auto ec = torc::svc::start(config);

    return static_cast<std::int16_t>(ec);
  } catch (const std::bad_optional_access &boaex) {
    std::cout << "Check your torc.cfg file" << std::endl;
    return (EXIT_FAILURE);
  }

  return (EXIT_SUCCESS);
}
