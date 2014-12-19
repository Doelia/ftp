#ifndef FILESENDER_H
#define FILESENDER_H

class FileSender;

#include "header-commun.h"

using namespace std;

class FileSender {

public:
	static FileSender* getInstance();
	static void init(ProgressNotifier*);

	FileSender(ProgressNotifier*);
	void startSendFile(string, Connexion*);
	void startSendFile_threaded(string, Connexion*);
	
private:
	static FileSender* instance;
	bool sendPartFile(string, char*, int, Connexion*);

	

};

#endif
