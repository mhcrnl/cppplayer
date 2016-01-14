#include "config.h"

using namespace std;
namespace po = boost::program_options;


Config::Config(string f) {
    file.open(f);
}

Config::~Config() {
  file.close();
}

void Config::Read(po::options_description& desc,
                        po::variables_map& vm)
{
  // Clear the map.
  vm = po::variables_map();
  po::store(po::parse_config_file(file , desc), vm);
  po::notify(vm);    
}
