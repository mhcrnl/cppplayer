#include "config.h"

using namespace std;
namespace po = boost::program_options;


void Config::Load(string f) {
  file.open(f);
    if(!file.is_open())
      throw runtime_error("File "+f+" could not be open");
}

Config::~Config() {
  file.close();
}

void Config::Read(options_description& desc)
{
  // Clear the map.
  variables_map vm = po::variables_map();
  store(po::parse_config_file(file , desc), vm);
  notify(vm);    
}
