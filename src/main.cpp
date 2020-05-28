#include <cstdint>
#include <iostream>
#include <optional>

#include "../include/libconfig.hpp"
#include "../include/config.hpp"
#include "../include/server.hpp"

int main(int argc, char **argv)
{
  try {
    auto config = torc::cfg::create().value();

    auto ec = torc::svc::start(config);

    return static_cast<std::int16_t>(ec);
  } catch (const std::bad_optional_access &boaex) {
    std::cout << "Check your torc.cfg file" << std::endl;
    return (EXIT_FAILURE);
  }

  return (EXIT_SUCCESS);
}
