#include "tools.h"

void initBuffer(char** buff, int size) {
	*buff = (char*) malloc(sizeof(char) * size + 1);
	for (int i =0; i < size+1; i++) {
		buff[0][i] = '\0';
	}
}

void getNext(const char* s, string delim) {
	
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

string getProgressBar(int pourcent) {
	int nbrBars = 40;
	string s = "[";
		int nbrBarsActives = pourcent * nbrBars / 100;

		for (int i = 0; i < nbrBarsActives; ++i) {
		 s += "=";
	 }

	 s += ">";

	 for (int i = nbrBarsActives; i < nbrBars; ++i) {
		 s += " ";
	 }

	 s += "] ";


char px[4];
sprintf(px, "%d", pourcent);
s += px;

s += "%";

return s;

}
