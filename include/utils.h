#pragma once

#include <fstream>
#include <string>

void daemonize();

std::ifstream::pos_type filesize(std::string filename);