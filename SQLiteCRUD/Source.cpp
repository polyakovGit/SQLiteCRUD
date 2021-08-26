#include <iostream>
#include "sqlite3.h"


using namespace std;

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; ++i) {
		//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::cout << azColName[i] << ' ' << argv[i] ? argv[i] : "NULL";
		std::cout << std::endl;
	}
	return 0;
}

int main() {
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open("example.db", &DB);
	if (exit) {
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		std::cout << "Opened Database Successfully!" << std::endl;

	char* messaggeError;
	std::string sql = "CREATE TABLE IF NOT EXISTS PERSON("
		"ID INTEGER PRIMARY KEY, "
		"NAME           TEXT	   NOT NULL, "
		"SURNAME          TEXT     NOT NULL, "
		"AGE            INTEGER    NOT NULL, "
		"ADDRESS        CHAR(50), "
		"SALARY         REAL )"
		";";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error Create Table" << std::endl;
		sqlite3_free(messaggeError);
	}
	else std::cout << "Table created Successfully" << std::endl;

	string query = "SELECT * FROM PERSON;";

	cout << std::endl << "STATE OF TABLE BEFORE INSERT" << endl;

	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

	sql = ("INSERT  INTO PERSON (NAME,SURNAME,AGE,ADDRESS,SALARY ) VALUES"
		"('STEVE', 'GATES', 30, 'PALO ALTO', 1000.0),"
		"('BILL', 'ALLEN', 20, 'SEATTLE', 300.22),"
		"('PAUL', 'JOBS', 24, 'SEATTLE', 9900.0);");

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Records created Successfully!" << std::endl;

	cout << std::endl << "STATE OF TABLE AFTER INSERT" << endl;

	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

	sql = "DELETE FROM PERSON WHERE ID = 1;";
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error DELETE" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Record deleted Successfully!" << std::endl;

	cout << std::endl << "STATE OF TABLE AFTER DELETE OF ELEMENT" << endl;
	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

	string data("CALLBACK FUNCTION");

	sql = ("SELECT * FROM PERSON;");
	if (exit) {
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
		std::cout << "Opened Database Successfully!" << std::endl;

	exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);

	if (exit != SQLITE_OK)
		cerr << "Error SELECT" << endl;
	else {
		cout << "Operation OK!" << endl;
	}

	sqlite3_close(DB);
	return 0;
}