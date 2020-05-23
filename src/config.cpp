#include <cstdlib>
#include <cinttypes>
#include <filesystem>
#include <optional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../include/config.hpp"
#include "../include/libconfig.hpp"

namespace fs = std::filesystem;    

std::optional<torc::cfg::Base> torc::cfg::create() 
{
    auto cfg_path = home_dir() / "torc.cfg";

    libconfig::Config cfg;

    try
    {
        cfg.readFile(cfg_path);
    }
    catch (const libconfig::FileIOException &fioex)
    {
        std::cerr << "io error while reading" << std::endl;
        return {};
    }
    catch (const libconfig::ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                    << " - " << pex.getError() << std::endl;  
        return {};
    }

    // read config
    try
    {
        std::unordered_map<std::string, torc::cfg::Proc> procs;

        // lookup port
        const std::uint32_t port = cfg.lookup("port");

        // lookup processes
        const libconfig::Setting &processes = cfg.getRoot()["processes"];

        const int p_count = processes.getLength();

        for (int i = 0; i < p_count; i++)
        {
            const libconfig::Setting &proc = processes[i];

            std::string name;
            std::uint32_t cnt;
            std::string cmd;

            if (!(
                    proc.lookupValue("name", name) &&
                    proc.lookupValue("threads", cnt) &&
                    proc.lookupValue("cmd", cmd)
                    ))
            {
                // return no-config even if a single proc isn't configured properly
                return {};
            }

            procs.insert({name, torc::cfg::Proc { name, cnt, cmd }});
        }

        return torc::cfg::Base{ port, procs };
    }
    catch (const libconfig::SettingNotFoundException &snfex)
    {
        return {};
    }

    return {};
}

fs::path torc::cfg::home_dir()
{
    auto torc_home = std::getenv("TORC_HOME");

    if (torc_home == NULL)
    {
        return fs::path {"./"};
    }
    else
    {
        return fs::path { torc_home };
    }
}
