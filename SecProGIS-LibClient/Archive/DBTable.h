#pragma once
#include "BWord.h"
#include <string.h>


//*******************
// CLASS TABLE CELL
//*******************

class T_CELL: public BASE_WORD{};


//*******************
// CLASS TABLE ROW
//*******************

class T_ROW {
private:
	
	T_CELL * cell;
	
	int size;

public:

	// CREATE EMPTY OBJECT
	T_ROW();

	// CREATE FROM AN ARRAY( SIZE OF "_size" )  OF STRING
	T_ROW(const char* const *src, int _size);

	// FROM ANOTHER OBJ (COPY CONSTRU)
	T_ROW(const T_ROW & src_ob);

	// DESTRU
	~T_ROW();

	// 0 FOR EMPTY , 1 FOR NOT EMPTY , ERROR OTHERWISE
	int isempty() const;

	// SET EMPTY
	void clear();

	// READ CONT IN FROM AN ARRAY( SIZE OF "_size" ) OF STRING (0 FOR SUCCESS , ERROR OTHERWISE)
	int read_in(const char* const *src, int _size);

	// HOW MANY CELLS
	int get_size() const;

	// GET CELL
	T_CELL& operator[] (int i) const;
};


//****************
// CLASS ROW NODE
//****************

class ROW_NODE {
public:

	T_ROW row;

	ROW_NODE * next;	//		自己定义自己？

	ROW_NODE();

	~ROW_NODE();
};

//**************
// CLASS TABLE
//**************

class _TABLE {
private:
	
	ROW_NODE * rows;

	int size;

public:

	// CREATE EMPTY OBJ
	_TABLE();

	// DESTRU
	~_TABLE();

	// 0 FOR EMPTY , 1 FOR NOT EMPTY , ERROR OTHERWISE
	int isempty() const;

	// SET EMPTY
	void clear();

	// HOW MANY ROWS
	int get_size() const;

	// GET ROW
	T_ROW & operator [] (int i) const;

	// ADD A NEW ROW FROM AN ARRAY( SIZE OF "_size" ) OF STRING (0 FOR SUCCESS , ERROR OTHERWISE)
	int append(const char * const * src, int _size);
};


//*************
// COMMON FUNCs
//*************

// PRINT OUT THE WHOLE TABLE TO CONSOLE
void print_tab(const _TABLE & table);