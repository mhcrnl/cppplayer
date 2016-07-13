#include "manager.h"

#include <sys/stat.h>

#include <iostream>
#include <exception>
#include <string>

int main(int argc, char* argv[]) try {
    Manager manager(argc, argv);
    manager.StartServer();
} catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
}
