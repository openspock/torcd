#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <optional>
#include <filesystem>
#include <string>
#include <vector>
#include <cinttypes>

namespace torc {
    namespace cfg {

        struct Proc
        {
            std::string     p_name; // name entry
            std::uint32_t   p_t_cnt; // count of parallel executions
            std::string     p_cmd; // command to be executed
        };

        struct Base
        {
            std::uint32_t       b_port;
            std::vector<Proc>   b_procs;
        };

        // factory function to create a base config
        std::optional<Base> create();

        // returns the home directory of torc -
        //
        // if TORC_HOME env var is set, then return that, or
        // else return current dir
        std::filesystem::path home_dir();
    }
}

#endif
