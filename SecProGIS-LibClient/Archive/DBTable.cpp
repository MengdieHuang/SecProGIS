#include "stdafx.h"
#include "DBTable.h"

//*******************
// CLASS TABLE ROW
//*******************

T_ROW::T_ROW(){
	cell = NULL;
	size = 0;
}

T_ROW::T_ROW(const char * const * src, int _size){
	if (read_in(src, _size) != 0) {
		cell = NULL;
		size = 0;
	}
}

T_ROW::T_ROW(const T_ROW & src_ob){
	if (src_ob.isempty() == 1) {
		size = src_ob.get_size();
		cell = new T_CELL[size];
		for (int i = 0; i < size; ++i) {
			cell[i] = src_ob[i];
		}
	}
}

T_ROW::~T_ROW(){
	if (cell) {
		delete[] cell;
	}
}

int T_ROW::isempty() const {
	if (size <= 0) {
		if (!cell) {
			return 0;
		}
		return -1;
	}
	if (!cell) {
		return -2;
	}
	return 1;
}

void T_ROW::clear(){
	size = 0;
	if (cell) {
		delete[] cell;
		cell = NULL;
	}
}

int T_ROW::read_in(const char * const * src, int _size)
{
	if (_size <= 0) {
		size = 0;
		cell = NULL;
	}
	else {
		if (!src) {
			size = 0;
			cell = NULL;
		}
		else {
			clear();
			size = _size;
			cell = new T_CELL[size];
			for (int i = 0; i < size; ++i) {
				int cell_len = src[i] ? strlen(src[i]) : 0;
				if (cell[i].read_in(src[i], cell_len) == 0) {
					// - - - action for empty read in - - -
				}
			}
		}
	}
	return size;
}

int T_ROW::get_size() const{
	return size;
}

T_CELL & T_ROW::operator [](int i) const{
	if (isempty() != 1) {
		// - - - 处理空指针调用[] - - -

	}
	i = ((i % size) + size) % size;
	return cell[i];
}


//****************
// CLASS ROW NODE
//****************

ROW_NODE::ROW_NODE(){
	next = NULL;
}

ROW_NODE::~ROW_NODE(){
	if (next) {
		delete next;
	}
}

//**************
// CLASS TABLE
//**************

_TABLE::_TABLE(){
	size = 0;
	rows = NULL;
}

_TABLE::~_TABLE(){
	if (rows) {
		delete rows;
	}
}

int _TABLE::isempty() const{
	if (size <= 0) {
		if (!rows) {
			return 0;
		}
		return -1;
	}
	if (!rows) {
		return -2;
	}
	return 1;
}

void _TABLE::clear(){
	size = 0;
	if (rows) {
		delete rows;
		rows = NULL;
	}
}

int _TABLE::get_size() const{
	return size;
}

T_ROW & _TABLE::operator[](int i) const
{
	if (!rows) {
		// - - - 处理空指针调用[] - - -
		
	}
	i = ((i % size) + size) % size;
	i = size - 1 - i;
	ROW_NODE * ret = rows;
	for (int j = 0; j < i; ++j) {
		ret = ret->next;
	}
	return ret->row;
}

int _TABLE::append(const char * const * src, int _size){
	ROW_NODE* nrow = new ROW_NODE();
	int read_ret = nrow->row.read_in(src, _size);
	if (read_ret == 0) {
		// - - - action for empty read in - - -
	}
	nrow->next = rows;
	rows = nrow;
	++size;
	return 0;
}


//*************
// COMMON FUNCs
//*************

void print_tab(const _TABLE & table){
	if (table.isempty() == 0) {
		printf(" - - - TABLE EMPTY - - - \n\n");
		return;
	}
	if (table.isempty() != 1) {
		printf(" - - - TABLE ERROR - - - \n\n");
		return;
	}
	printf(" - - - PRINTING TABLE - - -\n");
	for (int i = 0; i < table.get_size(); ++i) {
		for (int j = 0; j < table[i].get_size(); ++j) {
			print_dat(table[i][j].get_cont(), table[i][j].get_len());
			printf(", ");
		}
		printf("\n\n");
	}
	printf(" - - - PRINTING END - - -\n\n");
}
