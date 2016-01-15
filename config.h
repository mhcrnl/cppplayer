#include <fstream>
#include <string>
#include <boost/program_options.hpp>

using namespace boost::program_options;

class Config {
	public:
		void Load(std::string);
		~Config();
		void Read(options_description& desc);
	private:
		std::ifstream file;
};