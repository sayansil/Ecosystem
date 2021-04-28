#include <logger.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace e_logger
{
    spdlog::sink_ptr console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::sink_ptr error_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    spdlog::logger logger = spdlog::logger("default_logger", console_sink);
    
    void init_loggers()
    {
        console_sink->set_pattern("%v");
        error_sink->set_pattern("%^[error]:%$ %v");
    }
};
