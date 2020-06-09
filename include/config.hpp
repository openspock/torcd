#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <optional>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <cinttypes>

namespace torc::cfg {
struct Proc
{
  std::string p_name;// name entry
  std::uint32_t p_t_cnt;// count of parallel executions
  std::string p_cmd;// command to be executed
  std::uint32_t p_argc;// no of cmd line args
};

struct Base
{
  std::uint32_t b_port;
  std::unordered_map<std::string, Proc> b_procs;
};

// factory function to create a base config
std::optional<Base> create();

// returns the home directory of torc -
//
// if TORC_HOME env var is set, then return that, or
// else return current dir
std::filesystem::path home_dir();
}// namespace torc::cfg

#endif
