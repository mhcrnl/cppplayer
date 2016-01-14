#include <boost/filesystem.hpp>


using namespace boost::filesystem;
using namespace std;


class List {
	public:
		void LoadFrom(path p);
		void Randomize();
		const vector<path>& Get() const;
	private:
		vector<path> songList;
};