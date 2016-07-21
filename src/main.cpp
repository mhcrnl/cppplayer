#include <spdlog/spdlog.h>


#include "manager.h"

#include <iostream>
#include <exception>
#include <string>

#if !defined(_NAMED_PIPE) && !defined(_TCP_SOCKET)
#error At least we need one protocol to use
#endif

int main(int argc, char* argv[]) try {
    auto logging = spdlog::stdout_logger_mt("global", true);
    logging->set_level(spdlog::level::trace);
    logging->info("Starting server");

    Manager manager(argc, argv);
    manager.StartServer();
} catch(std::exception& e) {
    spdlog::get("global")->critical(e.what());
}
