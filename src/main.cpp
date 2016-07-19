#include <spdlog/spdlog.h>


#include "manager.h"

#include <sys/stat.h>

#include <iostream>
#include <exception>
#include <string>

int main(int argc, char* argv[]) try {
    auto logging = spdlog::stdout_logger_mt("global", true);
    logging->set_level(spdlog::level::debug);
    logging->info("Starting server");

    Manager manager(argc, argv);
    manager.StartServer();
} catch(std::exception& e) {
    spdlog::get("global")->critical(e.what());
}
