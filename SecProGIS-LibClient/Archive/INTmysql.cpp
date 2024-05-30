#include "stdafx.h"
#include "INTmysql.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif // DEBUG

char MY_QUERY::DB_IP[20] = { '\0' };
int MY_QUERY::DB_PORT = -1;
char MY_QUERY::DB_USR[20] = { '\0' };
char MY_QUERY::DB_PWD[20] = { '\0' };
char MY_QUERY::DB[20] = { '\0' };

int _query(MYSQL *con, const char *str) {
	return mysql_real_query(con, str, strlen(str));
}

int MY_QUERY::set_conf(const char* ip,
	int port, const char* usr,
	const char* pwd, const char* db){
	if (!(usr && pwd && ip && db)) {
		return -1;
	}
	strcpy_s(DB_USR, usr);
	strcpy_s(DB_PWD, pwd);
	strcpy_s(DB_IP, ip);
	strcpy_s(DB, db);
	DB_PORT = port;
	return 1;
}

bool MY_QUERY::configured(){
	if (!(strlen(DB_IP) == 0)
		&& !(DB_PORT == -1)
		&& !(strlen(DB_USR) == 0)
		&& !(strlen(DB_PWD) == 0)
		&& !(strlen(DB) == 0)) {
		return true;
	}
	return false;
}

int MY_QUERY::exec_query_mysql(const char * sql_str){
	if (!configured()) {
		return -1;
	}
	if (!sql_str) {
		return -1;
	}
	MYSQL *con = mysql_init(NULL);
	if (!con) {
		return -2;
	}
	if (!mysql_real_connect(con, DB_IP, DB_USR, DB_PWD, DB, DB_PORT, NULL, 0)) {
		mysql_close(con);
		return -3;
	}
	if (_query(con, "set names gbk;")) {
		mysql_close(con);
		return -4;
	}
	if (_query(con,sql_str)) {
		mysql_close(con);
		return -4;
	}
	mysql_close(con);
	return 0;
}

int MY_QUERY::exec_query_mysql(const char * sql_str, _TABLE & ret_table){
	if (!configured()) {
		return -1;
	}if (!sql_str) {
		return -1;
	}
	MYSQL mysql;
	MYSQL *con = mysql_init(&mysql);
	if (!con) {
		return -2;
	}
	if (!mysql_real_connect(con, DB_IP, DB_USR, DB_PWD, DB, DB_PORT, NULL, 0)) {
		mysql_close(con);
		return -3;
	}
	if (_query(con, "set names \'GBK\';")) {
		mysql_close(con);
		return -4;
	}
	if (_query(con, sql_str)) {
		return -4;
	}
	MYSQL_RES *res = mysql_store_result(con);
	MYSQL_ROW row;
	int cols;
	while (row = mysql_fetch_row(res))
	{
		cols = mysql_num_fields(res);
		if (ret_table.append(row, cols) != 0) {
			mysql_free_result(res);
			mysql_close(con);
			return -5;
		}
#ifdef DEBUG
		for (int i = 0; i < cols; ++i) {
			cout << row[i] << " , ";
		}
		cout << endl;
#endif
	}
	mysql_free_result(res);
	mysql_close(con);
	return 0;
}
