#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>
#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

#include "./config.hpp"

namespace torc {
    namespace svc {

        typedef std::shared_ptr<std::atomic_uint32_t> atomic_ptr_t;
        typedef std::unordered_map<std::string, atomic_ptr_t> atomic_umap_t;

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
        void connection_handler(std::uint64_t, const torc::cfg::Base&, const atomic_umap_t&);
    }
}

#endif
