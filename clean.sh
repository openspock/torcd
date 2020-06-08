#!/bin/sh

#cleanup target and downloaded headers

rm ./include/libconfig.hpp
# download spdlog header
rm ./include/spdlog.h
rm ./include/common.h
rm ./include/logger.h
rm ./include/tweakme.h
rm ./include/formatter.h
rm ./include/async_logger.h
rm ./include/sinks/base_sink.h
rm ./include/sinks/sink.h
rm ./include/sinks/file_sinks.h
rm ./include/sinks/stdout_sinks.h
rm ./include/sinks/ansicolor_sink.h
rmdir include/sinks
rm ./include/details/log_msg.h
rm ./include/details/os.h
rm ./include/details/null_mutex.h
rm ./include/details/pattern_formatter_impl.h
rm ./include/details/logger_impl.h
rm ./include/details/spdlog_impl.h
rm ./include/details/registry.h
rm ./include/details/async_logger_impl.h
rm ./include/details/async_log_helper.h
rm ./include/details/mpmc_blocking_q.h
rm ./include/details/file_helper.h
rmdir include/details
rm ./include/fmt/fmt.h
rm ./include/fmt/bundled/printf.h
rm ./include/fmt/bundled/ostream.h
rm ./include/fmt/bundled/format.h
rm ./include/fmt/bundled/format.cc
rm ./include/fmt/bundled/time.h
rm ./include/fmt/bundled/posix.h
rmdir include/fmt/bundled
rmdir include/fmt

make clean
