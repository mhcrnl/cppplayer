#include <spdlog/spdlog.h>


#include "manager.h"
#include "config.h"

#include <iostream>
#include <exception>
#include <string>

#if !defined(_NAMED_PIPE) && !defined(_TCP_SOCKET)
#error At least we need one protocol to use
#endif

int main(int argc, char* argv[]) try {
    auto logging = spdlog::stdout_logger_mt("global", true);
    logging->set_level(spdlog::level::trace);

    //Check if we have some pid number
    std::ifstream ipid_file(Config().GetPidFile());
    if(ipid_file.is_open()) {

        std::string pid;
        ipid_file >> pid;

        //Check if that pid is a real process
        std::ifstream f("/proc/"+pid+"/comm");
        if(f.is_open()) {
            std::string comm;
            f >> comm;
            if(comm == "dplayer++") {
                throw std::runtime_error("Server is already running");
            }
        }
    }

    logging->info("Starting server");

    Manager manager(argc, argv);
    manager.StartServer();
} catch(std::exception& e) {
    spdlog::get("global")->critical(e.what());
}
