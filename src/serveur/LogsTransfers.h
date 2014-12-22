#ifndef LOGS_H
#define LOGS_H

#include "header-serveur.h"

class LogsTransfers : public ProgressNotifier {

public:

	LogsTransfers();

	virtual void onFileStart(string, int, int);
	virtual void onFileProgress(string nameFile, int pourcent, int);
	virtual void onFileEnd(string, int);

private:
	
};

#endif
