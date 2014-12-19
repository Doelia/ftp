#include "tools.h"

void initBuffer(char** buff, int size, bool aloc) {
	if (aloc) // TODO
		*buff = (char*) malloc(sizeof(char) * size + 1);
	for (int i =0; i < size+1; i++) {
		buff[0][i] = '\0';
	}
}

void initBuffer(char** buff, int size) {
	initBuffer(buff, size, true);
}

void split(const string &s, string delim, vector<string> &elems) {
	int x = s.find (delim, 0);
	if (x == -1) {
		elems.push_back(s);
	} else {
		string elem = s.substr(0, x);
		elems.push_back(elem);
		string next = s.substr(x+delim.size(), s.size());
		split(next, delim, elems);
	}
}

vector<string> split(const string &s, string delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

