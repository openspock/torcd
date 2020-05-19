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
      std::cout << config.b_procs[0].p_name << std::endl;
  }
  catch (const std::bad_optional_access& boaex)
  {
      std::cout << "Check your torc.cfg file" << std::endl;
      return(EXIT_FAILURE);
  }

  return(EXIT_SUCCESS);
}
