#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>
#include <atomic>
#include <string>
#include <unordered_map>

#include "./config.hpp"

namespace torc {
    namespace svc {

        enum class exitcode: std::int16_t
        {
            boot_failure = -1,
            graceful_shutdown
        };

        // starts the server with the passed config
        exitcode start(const torc::cfg::Base cfg);

        // Handles connection for ever new client socket in a new
        // thread.
        //
        // Args
        // 
        // socket descriptor
        // reference to base cfg
        // reference to a map of proc and thread count
        void connection_handler(std::uint64_t, const torc::cfg::Base&, const std::unordered_map<std::string, std::atomic_uint32_t>&);
    }
}

#endif
