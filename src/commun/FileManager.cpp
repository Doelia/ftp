#include "FileManager.h"

FileManager* FileManager::instance = NULL;

FileManager* FileManager::getInstance() {
	if (FileManager::instance == NULL) {
		perror("Erreur, file manager non initialisé");
		stop();
	}
	return FileManager::instance;
}

FileManager* FileManager::init(string dir) {
	FileManager::instance = new FileManager();
	FileManager::instance->dirFiles = dir;
	FileManager::instance->createDirectoryIfNotExists(dir);
	return FileManager::instance;
}

bool FileManager::fileExistsInDir(string nameFile) {
	string realName = this->dirFiles + nameFile;
	return this->fileExists(realName);
}

bool FileManager::fileExists(string nameFile) {
	return (access(nameFile.c_str(), F_OK ) != -1);

}

int FileManager::getSize(string nameFile) {
	string realName = this->dirFiles + nameFile;
	FILE *file;
	if ((file = fopen(realName.c_str(), "r"))) {
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);
		fclose(file);
		return size;
	}
	cout << "Erreur. Taille de fichier introuvable." << endl;
	return -1;
}

int FileManager::createFileInDir(string nameFile) {
	return this->createFile(this->dirFiles + nameFile);
}


int FileManager::createFile(string nameFile) {
	int fd = open(nameFile.c_str(), O_CREAT | O_WRONLY);
	if (fd < 0) {
		string s = "Erreur lors de la création du fichier " + nameFile;
		perror(s.c_str());
		stop();
	}
	return fd;
}

int FileManager::openFileInDir(string nameFile) {
	string realName = nameFile + this->dirFiles;
	int fd = open(realName.c_str(), O_RDONLY);
	if (fd < 0) {
		string s = "Erreur lors de l'ouverture du fichier " + nameFile;
		perror(s.c_str());
		stop();
	}
	return fd;
}

void FileManager::createDirectoryIfNotExists(string name) {
	struct stat st = {0};
	if (stat(name.c_str(), &st) == -1) {
	    mkdir(name.c_str(), 0700);
	}
}


