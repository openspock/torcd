#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <optional>

#include "../include/libconfig.hpp"
#include "../include/config.hpp"

int main(int argc, char **argv)
{
  try
  {
      auto config = torc::cfg::create().value();
      std::cout << config.procs[0].cf_name << std::endl;
  }
  catch (const std::bad_optional_access& boaex)
  {
      return(EXIT_FAILURE);
  }

  return(EXIT_SUCCESS);
}
