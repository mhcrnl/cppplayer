#include <boost/filesystem.hpp>


using namespace boost::filesystem;
using namespace std;


class List {
	public:
		void LoadFrom(path p);
		void Randomize();
		const vector<path>& Get() const;
		bool SaveCache(string filename);
		bool LoadCache(string filename); 
	private:
		vector<path> songList;
};