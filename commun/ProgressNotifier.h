#ifndef PN_H
#define PN_H

class ProgressNotifier;

#include "header-commun.h"

using namespace std;

class ProgressNotifier {

public:
	ProgressNotifier();
	void onFileStart(string);
	void onFileProgress(string nameFile, int pourcent);
private:

};

#endif
