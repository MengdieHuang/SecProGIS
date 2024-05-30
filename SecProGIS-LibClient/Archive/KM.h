#pragma once

#include <time.h>
#include <stdlib.h>
#include "BWord.h"
#include "INTmysql.h"

//*****************
//BASE CLASS
//    for 
//DATA SEGMENT
//*****************
class KM_WORD : public BASE_WORD {
public:
	KM_WORD();

	KM_WORD(const char *src);

	KM_WORD(const char *src, int l);

	KM_WORD(const KM_WORD& src_ob);

	~KM_WORD();
};


//*****************
//SK CLASS
//*****************
class KM_SK : public KM_WORD {
public:
	int generate(int _len);
};


//*****************
//SERVICE CLASS
//     for
//each FILE & KEY
//*****************
class KM_SERV {
private:
	int orgnization;
	KM_WORD md5_plaint;
	KM_WORD md5_cipher;
	KM_SK skey;
	time_t expire_t;
	time_t enc_t;

	static char SK_TABLE[20];
	static int admin;
public:

	// CONSTRU ( USING ORGNIZATION AND MD5 CIPHER CODE )
	KM_SERV(int comp, const KM_WORD &md5_c);

	// CONSTRU ( USINT ORGNIZATION , MD5 PLAINT CODE AND EXPIRE TIME )
	KM_SERV(int comp, const KM_WORD &md5_p, time_t expire);

	// GENERATE KEY IN LENGH "len"
	int gen_key(int len);

private:
	int store();
public:
	// STORE SERVICE DATA ( ALL DATA NOT EMPTY )
	int store(const KM_WORD& md5_c, time_t enctm);

	// RESTORE SERVICE DATA ( WITH "orgnization" & "md5_cipher" AVAILABLE )
	int restore();

private:
	int verify();
public:
	// COPY "skey" TO "dest" ( SERVICE NOT EXPIRED )
	int get_skey(char *dest);

	// SET SERVIEC KEY TABLE NAME
	static int set_conf(const char *sk_tab, int adm);

	// IF SERVICE KEY TABLE NAME SET
	static bool configured();
};