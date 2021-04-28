#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace e_logger
{
    extern spdlog::logger logger;
    extern spdlog::sink_ptr console_sink;
    extern spdlog::sink_ptr error_sink;
    extern void init_loggers();
};

#endif /* LOGGER_HPP */
