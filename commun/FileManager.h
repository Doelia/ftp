#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager;

#include "header-commun.h"

class FileManager {

public:
	static FileManager* getInstance();
	static FileManager* init(string);

	bool exists(string);
	bool keyExists(string);
	int getSize(string);
	int createKey(string);
	void startSendFile(string, Connexion*);
	void startSendFile_threaded(string, Connexion*);
	int createFile(string);  

private:
	string dirFiles;
	static FileManager* instance;
	bool sendPartFile(string, char*, int, Connexion*);

};

#endif
