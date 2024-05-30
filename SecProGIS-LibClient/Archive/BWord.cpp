#include "stdafx.h"
#include "BWord.h"

//*****************
// BASE CLASS WORD 
//*****************

BASE_WORD::BASE_WORD() {
	content = NULL;
	len = 0;
}

BASE_WORD::BASE_WORD(const char * src) {
	len = 0;
	content = NULL;
	read_in(src, strlen(src));
}

BASE_WORD::BASE_WORD(const char * src, int l) {
	len = 0;
	content = NULL;
	read_in(src, l);
}

BASE_WORD::BASE_WORD(const BASE_WORD & src_ob) {
	len = 0;
	content = NULL;
	if (src_ob.isempty() == 1) {
		len = src_ob.get_len();
		content = new char[len+3];
		memset(content, '\0', len + 3);
		src_ob.get_cont(content);
	}
}

BASE_WORD::~BASE_WORD() {
	if (content) {
		delete[] content;
	}
}

int BASE_WORD::isempty() const {
	if (len <= 0) {
		if (content == NULL) {
			return 0;
		}
		return -1;
	}
	if (content == NULL) {
		return -2;
	}
	return 1;
}

void BASE_WORD::clear() {
	len = 0;
	if (content) {
		delete[] content;
		content = NULL;
	}
}

int BASE_WORD::read_in(const char * src, int l) {
	if (l <= 0) {
		len = 0;
		content = NULL;
	}
	else {
		if (!src) {
			len = 0;
			content = NULL;
		}
		else {
			clear();
			len = l;
			content = new char[len + 3];
			memset(content, '\0', len + 3);
			memcpy(content, src, len);
		}
	}
	return len;
}

const BASE_WORD & BASE_WORD::operator=(const BASE_WORD & src_ob){
	clear();
	if (src_ob.isempty() == 1) {
		len = src_ob.get_len();
		content = new char[len+3];
		memset(content, '\0', len + 3);
		src_ob.get_cont(content);
	}
	return *this;

}

char BASE_WORD::operator[](int i) const {
	if (isempty() != 1) {
		return '\0';
	}
	i = ((i % len) + len) % len;
	return content[i];
}

int BASE_WORD::get_len() const
{
	return len;
}

int BASE_WORD::get_cont(char *dest) const {
	if (isempty() != 1) {
		return -1;
	}
	if (!dest) {
		return  -2;
	}
	memcpy(dest, content, len);	//generate的content拷贝到dest,作为密钥
	return 0;
}

int BASE_WORD::get_cont(char * dest, int maxlen) const{
	if (isempty() != 1) {
		return -1;
	}
	if (!dest) {
		return  -2;
	}
	maxlen = maxlen < len ? maxlen : len;
	memcpy(dest, content, maxlen);
	return 0;
}

const char * BASE_WORD::get_cont() const{
	return content;
}


//**************
// COMMOM FUNCs
//**************

void print_dat(const char * src, int maxlen) {
	char *tmp = new char[maxlen + 3];
	memset(tmp, '\0', maxlen + 3);
	memcpy(tmp, src, maxlen);
	printf("%s", tmp);
	delete tmp;
}

void sprint_hex(const char* src, int maxlen, char* dest) {
	int temp;
	for (int i = 0; i < maxlen; ++i) {
		temp = (int)(unsigned char)src[i];
		sprintf_s(dest + 2 * i, 3, "%02x", temp);
	}
}