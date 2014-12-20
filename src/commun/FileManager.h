#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager;

#include "header-commun.h"

class FileManager {

public:
	static FileManager* getInstance();
	static FileManager* init(string);

	bool fileExistsInDir(string);
	bool fileExists(string);
	int getSize(string);
	
	int createFileInDir(string);  
	int createFile(string);  
	int openFileInDir(string);

	void createDirectoryIfNotExists(string name);

private:
	string dirFiles;
	static FileManager* instance;

};

#endif
