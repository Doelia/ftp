#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager;

#include "header-commun.h"

class FileManager {

public:
	static FileManager* getInstance();
	static FileManager* init();

	bool exists(string);
	int getSize(string);
	int createFile(string);
	void startSendFile(string, Connexion*);

private:
	static FileManager* instance;
	void sendPartFile(string, char*, int, Connexion*);

};

#endif
