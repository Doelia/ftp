#ifndef TOOLS_H
#define TOOLS_H

#include "header-commun.h"


using namespace std;

void initBuffer(char** buff, int size);
void initBuffer(char** buff, int size, bool);
vector<string> split(const string &s, string delim);
string getProgressBar(int);

#endif