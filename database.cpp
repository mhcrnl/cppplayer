#include <iostream>
#include "database.h"

//SQLite C++ resources: 
//	https://www.sqlite.org/cintro.html
//	https://www.sqlite.org/c3ref/intro.html

Database::Database() : isConnected(false) {
}

Database::~Database() {
	if (isConnected) {
		sqlite3_close(db);
	}
}

bool Database::Connect(const char* path){
	//Connects to the database,
	//Returns true if the connection was successful, and false if not

	int rc = sqlite3_open(path, &db);

	if (rc) {
		isConnected = false;
		return false;
	} else {
		isConnected = true;
		return true;
	}
}

bool Database::Exec(char *text) {
	//Runs an SQL query, returns false if it fails

	query.sql = text;
	sqlite3_initialize();
	//https://www.sqlite.org/c3ref/prepare.html
	int ret = sqlite3_prepare(db, query.sql, -1, &query.stmt, nullptr);
	if (ret != SQLITE_OK) {
		//Handle error
		return false;
	}

	int status = sqlite3_step(query.stmt);
	//TODO: Handle error depending on error code
	//https://www.sqlite.org/c3ref/step.html
	if (status != SQLITE_DONE) {
		return false;
	}

	sqlite3_finalize(query.stmt);
	return true;
} 



