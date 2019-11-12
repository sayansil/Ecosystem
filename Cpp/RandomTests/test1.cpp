#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main()
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("logger");
    logger->set_pattern("%^[%n] [%l] [%v]%$");
    logger->info("Going fine!!!");
    logger->error("Going wrong!!!");
    logger->warn("Warning");
    logger->debug("Debug");
    logger->trace("trace");
    logger->critical("critical");
    spdlog::get("logger")->info("Logger found!!!");

}
