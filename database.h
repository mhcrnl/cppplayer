#include <sqlite3.h>

class Database {
public:
	~Database();
	bool Connect(const char*);
	bool Exec(char*);

private:
	struct Query{
		//Struct containing vars involved in sql queries
		//TODO: Binding params
		sqlite3_stmt* stmt;
		char* sql;
	}query;

	sqlite3* db;

	bool isConnected {false};
};
