#include <sqlite3.h>

#define TITLE_QUERY "SELECT title FROM songs WHERE spath=?"

class Database {
public:
	~Database();
    void Connect(const char*);
	bool Exec(char*);
	bool GetTitleFromDB(const std::string, std::string&);
private:
	sqlite3_stmt* stmt;
	sqlite3* db;
};
