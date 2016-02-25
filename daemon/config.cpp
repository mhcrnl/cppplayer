#include "config.h"

using namespace std;


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
  variables_map vm = variables_map();
  store(parse_config_file(file , desc), vm);
  notify(vm);    
}

Options Config::GetConfig() {
	Options opt;
	string home = getenv("HOME");
	if(home.empty()) 
		throw runtime_error("HOME env variable not found");

	Load(home+"/.config/player++");

	options_description desc("Options");
	desc.add_options()
	    ("daemon_pipe", value<string>(&opt.daemonpipe))
	    ("client_pipe", value<string>(&opt.clientpipe))
	    ("music_folder", value<path>(&opt.dir))
	    ("auto_start", value<bool>(&opt.autostart))
	    ("cache_file", value<string>(&opt.cachefile))
		;
	Read(desc);
	return opt;
}
