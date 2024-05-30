#pragma once
#include <string.h>

//*****************
// BASE CLASS WORD 
//*****************

class BASE_WORD {
protected:

	char *content;
	
	int len;

public:

	// CREATE EMPTY OBJECT
	BASE_WORD();
	
	// CREAT OBJECT FORM a STRING
	BASE_WORD(const char *src);
	
	// FROM A SEGMENT OF DATA
	BASE_WORD(const char *src, int l);
	
	// FROM ANOTHER OBJECT (COPY CONSTRU)
	BASE_WORD(const BASE_WORD& src_ob);
	
	// DESTRU
	~BASE_WORD();
	
	// 0 FOR EMPTY , 1 FOR NOT , OTHER FOR ERROR
	int isempty() const;
	
	// SET EMPTY
	void clear();
	
	// READ CONTENT IN FROM A SEG OF DATA (0 FOR SUCCESS , ERROR OTHERWISE)
	int read_in(const char* src, int l);
	
	// ASSIGNMENT
	const BASE_WORD & operator=(const BASE_WORD& src_ob);

	//  GET CHAR
	char operator[](int i) const;
	
	// CONTENT LENGTH
	int get_len() const;
	
	// COPY ALL CONTENT TO DEST (0 FOR SUCCESS , ERROR OTHERWISE)
	int get_cont(char *dest) const;

	// COPY CONTENT AT LONGEST "maxlen" TO DEST
	int get_cont(char *dest, int maxlen) const;

	// CONSTANT REFERANCE TO CONTENT
	const char* get_cont() const;
};


//**************
// COMMOM FUNCs
//**************

// PRINT DATA in "src" WHICH HAVE NO "\0" SYMBOL AS END SIGNAL
void print_dat(const char* src, int maxlen);

void sprint_hex(const char* src, int maxlen, char* dest);