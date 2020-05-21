#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>

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
        void connection_handler(std::uint64_t);
    }
}

#endif
