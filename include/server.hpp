#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>
#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

#include "./config.hpp"

namespace torc::svc {

using atomic_ptr_t = std::shared_ptr<std::atomic_uint32_t>;
using atomic_umap_t = std::unordered_map<std::string, atomic_ptr_t>;

enum class exitcode : std::int16_t {
  boot_failure = -1,
  graceful_shutdown
};

// starts the server with the passed config
exitcode start(torc::cfg::Base cfg);

// Handles connection for ever new client socket in a new
// thread.
//
// Args
//
// socket descriptor
// reference to base cfg
// reference to a map of proc and thread count
void connection_handler(std::int32_t, torc::cfg::Base &, atomic_umap_t &);
}// namespace torc::svc

#endif
