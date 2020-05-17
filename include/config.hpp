#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <cinttypes>

namespace torc {
    namespace cfg {

        struct Config
        {
            std::string     cf_name; // name entry
            std::uint32_t   cf_t_cnt; // count of parallel executions
            std::string     cf_cmd; // command to be executed
        }
    }
}

#endif
