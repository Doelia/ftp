#include "FileManager.h"

FileManager* FileManager::instance = NULL;

FileManager* FileManager::getInstance() {
	return FileManager::instance;
}

FileManager* FileManager::init() {
	FileManager::instance = new FileManager();
	return FileManager::instance;
}

bool FileManager::exists(string nameFile) {
	FILE *file;
    if (file = fopen(nameFile.c_str(), "r")) {
        fclose(file);
        return 1;
    }
    return 0;
}

int FileManager::getSize(string nameFile) {
	FILE *file;
    if (file = fopen(nameFile.c_str(), "r")) {
    	fseek(file, 0L, SEEK_END);
    	int size = ftell(file);
        fclose(file);
        return size;
    }
    return -1;
}

