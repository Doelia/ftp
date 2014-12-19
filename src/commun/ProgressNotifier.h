#ifndef PN_H
#define PN_H

class ProgressNotifier;

#include "header-commun.h"

using namespace std;

class ProgressNotifier {

public:
	ProgressNotifier();
	virtual void onFileStart(string) = 0;
	virtual void onFileProgress(string nameFile, int pourcent) = 0;
	virtual void onFileEnd(string) = 0;
private:

};

#endif
