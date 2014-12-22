#ifndef PN_H
#define PN_H

class ProgressNotifier;

#include "header-commun.h"

using namespace std;

class ProgressNotifier {

public:
	ProgressNotifier();
	virtual void onFileStart(string, int, int) = 0;
	virtual void onFileProgress(string nameFile, int pourcent, int) = 0;
	virtual void onFileEnd(string, int) = 0;
private:

};

#endif
