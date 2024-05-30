#include "stdafx.h"
#include "KM.h"

//*****************
//BASE CLASS KM_WORD
//*****************

KM_WORD::KM_WORD() {}

KM_WORD::KM_WORD(const char * src) : BASE_WORD(src) {}

KM_WORD::KM_WORD(const char * src, int l) : BASE_WORD(src, l) {}

KM_WORD::KM_WORD(const KM_WORD & src_ob) : BASE_WORD(src_ob) {}

KM_WORD::~KM_WORD() {}


//*****************
//SK CLASS
//*****************

	int KM_SK::generate(int _len) {	//generate key
	time_t t;
	time(&t);
	/*printf("time(): %d\n", t);
	t = clock();
	printf("clock(): %d\n", t);*/
	srand((unsigned int)t);

	if (len != 0 || content) {
		//return -1;
		clear();
	}
	content = new char[_len+3];
	memset(content, '\0', _len + 3);
	for (int i = 0; i < _len; ++i) {
		int key1 = (rand() % 3 + 3) % 3;
		if (key1 == 0) {
			content[i] = (char)(((rand() % 10 + 10) % 10) + 48);
		}
		else if (key1 == 1) {
			content[i] = (char)(((rand() % 26 + 26) % 26) + 65);
		}
		else {
			content[i] = (char)(((rand() % 26 + 26) % 26) + 97);
		}
	}
	len = _len;
	//printf("content altered: %s\n",content);
	return 0;
}


//*****************
//SERVICE CLASS
//*****************

char KM_SERV::SK_TABLE[20] = { '\0' };

int KM_SERV::admin = 0;

KM_SERV::KM_SERV(int comp, const KM_WORD & md5_c){
	orgnization = comp;
	md5_cipher = md5_c;
}

KM_SERV::KM_SERV(int comp, const KM_WORD& md5_p, time_t expire) {
	orgnization = comp;
	md5_plaint = md5_p;
	expire_t = expire;

}

int KM_SERV::gen_key(int len) {
	return skey.generate(len);
}

int KM_SERV::store(){
	if (md5_plaint.isempty() != 1) {
		return -1;
	}if (md5_cipher.isempty() != 1) {
		return -1;
	}if (skey.isempty() != 1) {
		return -1;
	}if (expire_t == 0) {
		return -1;
	}if (!configured()) {
		return -1;
	}

	// - - - if data already exist  - - - 
	_TABLE table;
	char *sql = new char[200];
	sprintf_s(sql, 200, "select * from %s where md5_c = '%s';",
		SK_TABLE, md5_cipher.get_cont());
	if (MY_QUERY::exec_query_mysql(sql, table) != 0) {
		/*delete sql;
		return -2;*/
	}
	delete sql;
	
	char *skey_tmp = new char[2 * skey.get_len()+2];
	memset(skey_tmp, 0, 2 * skey.get_len());
	// - - - convert skey to hex format - - - -
	sprint_hex(skey.get_cont(), skey.get_len(), skey_tmp);
	// - - - store service data into database - - -
	sql = new char[300];
	if (table.get_size() < 1) {
		sprintf_s(sql, 300, "insert into %s (org, md5_p, md5_c, skey, expire, admin, enc_tm"
			") values ('%d','%s','%s','%s','%lld','%d','%lld'"
			");", SK_TABLE, orgnization, md5_plaint.get_cont(), md5_cipher.get_cont(), skey.get_cont(), expire_t, admin, enc_t);
	}
	else {
		sprintf_s(sql, 300, "update %s set org = '%d', md5_p = '%s', skey = '%s'"
			", expire = '%lld', admin = '%d', enc_tm = '%lld' where md5_c = '%s';", SK_TABLE, 
			orgnization, md5_plaint.get_cont(), skey.get_cont(), expire_t, admin, enc_t, md5_cipher.get_cont());
	}
	delete[] skey_tmp;

	if (MY_QUERY::exec_query_mysql(sql) != 0) {
		delete sql;
		return -2;
	}
	delete sql;

	return 0;
}

int KM_SERV::store(const KM_WORD& md5_c, time_t enctm) {
	if (md5_c.isempty() != 1) {
		return -1;
	}
	md5_cipher = md5_c;
	enc_t = enctm;
	if (store() != 0) {
		return -1;
	}
	return 0;
}

int KM_SERV::restore(){
	if (md5_cipher.isempty() != 1) {
		return -2;
	}if (!configured()) {
		return -2;
	}

	// - - - restore data from database - - -
	_TABLE table;
	char *sql = new char[200];
	sprintf_s(sql, 200, "select * from %s where md5_c = '%s' and org = '%d';", 
		SK_TABLE, md5_cipher.get_cont(), orgnization);
	if (MY_QUERY::exec_query_mysql(sql, table) != 0) {
		delete[] sql;
		return -3;
	}
	delete[] sql;

	if (table.get_size() <= 0 || table[0].get_size() < 5) {
		return -3;
	}

	if (md5_plaint.read_in(table[0][1].get_cont(), table[0][1].get_len()) == 0) {
		return -4;
	}
	// - - - HEX KEY CONVERT INTO U8 - - -
	const char *skey_h = table[0][3].get_cont();
	int key_len = table[0][3].get_len()/2;
	unsigned char *skey_uc = new unsigned char[key_len];
	for (int i = 0; i < key_len; i++)
	{
		int h1 = (int)((skey_h[2 * i] < 'a') ? (skey_h[2 * i] - '0') : (skey_h[2 * i] - 'a' + 10));
		h1 = h1 * 16;
		int h2 = (int)((skey_h[2 * i + 1] < 'a') ? (skey_h[2 * i + 1] - '0') : (skey_h[2 * i + 1] - 'a' + 10));
		skey_uc[i] = (unsigned char)(h1 + h2);
	}// - - - HEX KEY CONVERT INTO U8 - - -

	if (skey.read_in((char *)skey_uc, key_len) == 0) {
		return -4;
	}
	char *tmpexp = new char[16];
	memset(tmpexp, '\0', 16);
	if (table[0][4].get_cont(tmpexp, 16) != 0) {
		return -4;
	}
	expire_t = atoi(tmpexp);

	return 0;
}

int KM_SERV::verify(){
	if (expire_t == 0) {
		return -1;
	}
	time_t curr_t;
	time(&curr_t);
	if (expire_t - curr_t < 0) {
		return -2;
	}
	return 0;
}

int KM_SERV::get_skey(char *dest){
	if (verify() != 0) {
		return -1;
	}
	if (skey.get_cont(dest) != 0) {
		return -2;
	}
	return 0;
}

int KM_SERV::set_conf(const char * sk_tab, int adm){
	if (!sk_tab) {
		return -1;
	}
	admin = adm;
	strcpy_s(SK_TABLE, sk_tab);
	return 0;
}

bool KM_SERV::configured(){
	if (strlen(SK_TABLE) == 0) {
		return false;
	}
	return true;
}

