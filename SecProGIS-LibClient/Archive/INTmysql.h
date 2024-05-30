#pragma once
#include "mysql.h"
#include "DBTable.h"

//#define DEBUG

class MY_QUERY {
	
private:
	static char DB_USR[20];
	static char DB_PWD[20];
	static char DB_IP[20];
	static int DB_PORT;
	static char DB[20];

public:
	static int set_conf(const char* ip,
		int port, const char* usr,
		const char* pwd, const char* db);

	static bool configured();

	// - - - QUERY WITHOUT RETURN DATA
	static int exec_query_mysql(const char* sql_str);
	
	// - - - QUERY WITH RETURN DATA (STORED INTO ret_table)
	static int exec_query_mysql(const char* sql_str, _TABLE& ret_table);
};




