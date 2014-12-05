#include "tools.h"

void initBuffer(char** buff, int size) {
	*buff = (char*) malloc(sizeof(char) * size);
	for (int i =0; i < size; i++) {
		buff[0][i] = '\0';
	}
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
