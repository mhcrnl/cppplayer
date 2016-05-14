#include <iostream>
#include "database.h"

//SQLite C++ resources: 
//	https://www.sqlite.org/cintro.html
//	https://www.sqlite.org/c3ref/intro.html

Database::~Database() {
	if (isConnected) {
		sqlite3_close(db);
	}
}

/**
 * Connects to the database and save the sqlite3 var
 *
 * @param path Path where the database file is
 * @return True if success, false if there're any errors
 */
bool Database::Connect(const char* path){
	//Connects to the database,
	//Returns true if the connection was successful, and false if not

	int rc;
	rc = sqlite3_open(path, &db);

	if (rc) {
		isConnected = false;
		return false;
	} else {
		isConnected = true;
		return true;
	}
}

/**
 * Exec a SQL query
 *
 * @param text SQL query
 * @return True if success, false if there're any errors
 */
bool Database::Exec(char* text) {
	//Runs an SQL query, returns false if it fails

	sqlite3_initialize();
	//https://www.sqlite.org/c3ref/prepare.html
	int ret = sqlite3_prepare(db, text, -1, &stmt, nullptr);
	if (ret != SQLITE_OK) {
		//Handle error
		return false;
	}

	int status = sqlite3_step(stmt);
	//TODO(quim): Handle error depending on error code
	//TODO(quim): Continue if there's more than 1 row involved
	//https://www.sqlite.org/c3ref/step.html
	if (status != SQLITE_DONE) {
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
} 

/**
 * Search the title of a song in the database
 *
 * @param path Song to be searched
 * @param title String where the title will be saved if it's found
 * @return True if the song is found, false if not
 */
bool Database::GetTitleFromDB(const std::string path, std::string& title) {
	const char* query = TITLE_QUERY;
	const char* result;

	//Prepare the query
	if ( sqlite3_prepare(db, query, -1, &stmt, nullptr)  != SQLITE_OK ) {
		//Handle error
		return false;
	}

	//Bind params
	//https://www.sqlite.org/c3ref/bind_blob.html
	if ( sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ) {
		//Handle error
		return false;
	}

	int res = sqlite3_step(stmt);
	if (res == SQLITE_ROW) {
		result = (char*)sqlite3_column_text(stmt, 0);
	} else {
		return false;
	}
	title = std::string(result);

	sqlite3_finalize(stmt);
	return true;
}
