#!/bin/sh

LIB_CONFIG=libconfig-1.7.2

# init - initialize orcd

# headers
# download libconfig header
curl -sl https://raw.githubusercontent.com/hyperrealm/libconfig/master/lib/libconfig.h++ -o ./include/libconfig.hpp
# download spdlog header
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/spdlog.h -o ./include/spdlog.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/common.h -o ./include/common.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/logger.h -o ./include/logger.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/tweakme.h -o ./include/tweakme.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/formatter.h -o ./include/formatter.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/async_logger.h -o ./include/async_logger.h
mkdir -p include/sinks
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/sinks/base_sink.h -o ./include/sinks/base_sink.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/sinks/sink.h -o ./include/sinks/sink.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/sinks/file_sinks.h -o ./include/sinks/file_sinks.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/sinks/stdout_sinks.h -o ./include/sinks/stdout_sinks.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/sinks/ansicolor_sink.h -o ./include/sinks/ansicolor_sink.h
mkdir -p include/details
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/log_msg.h -o ./include/details/log_msg.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/os.h -o ./include/details/os.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/null_mutex.h -o ./include/details/null_mutex.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/pattern_formatter_impl.h -o ./include/details/pattern_formatter_impl.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/logger_impl.h -o ./include/details/logger_impl.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/spdlog_impl.h -o ./include/details/spdlog_impl.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/registry.h -o ./include/details/registry.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/async_logger_impl.h -o ./include/details/async_logger_impl.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/async_log_helper.h -o ./include/details/async_log_helper.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/mpmc_blocking_q.h -o ./include/details/mpmc_blocking_q.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/details/file_helper.h -o ./include/details/file_helper.h
mkdir -p include/fmt/bundled
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/fmt.h -o ./include/fmt/fmt.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/bundled/printf.h -o ./include/fmt/bundled/printf.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/bundled/ostream.h -o ./include/fmt/bundled/ostream.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/bundled/format.h -o ./include/fmt/bundled/format.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/bundled/format.cc -o ./include/fmt/bundled/format.cc
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/bundled/time.h -o ./include/fmt/bundled/time.h
curl -sl https://raw.githubusercontent.com/gabime/spdlog/master/include/spdlog/fmt/bundled/posix.h -o ./include/fmt/bundled/posix.h

# make target dir
mkdir -p target

# download and build libconfig
curl -sl https://hyperrealm.github.io/libconfig/dist/$LIB_CONFIG.tar.gz -o ./target/$LIB_CONFIG.tar.gz

cd target
tar -xzf $LIB_CONFIG.tar.gz

cd $LIB_CONFIG

./configure --prefix=$(pwd)

make

make install
