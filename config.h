#include <fstream>
#include <string>
#include <boost/program_options.hpp>


namespace po = boost::program_options;


class Config {
	public:
		Config(std::string);
		~Config();
		void Read(po::options_description& desc,
                  po::variables_map& vm);
	private:
		std::ifstream file;
};