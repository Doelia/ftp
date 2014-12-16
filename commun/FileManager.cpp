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
	if ((file = fopen(nameFile.c_str(), "r"))) {
		fclose(file);
		return 1;
	}
	return 0;
}

int FileManager::getSize(string nameFile) {
	FILE *file;
	if ((file = fopen(nameFile.c_str(), "r"))) {
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);
		fclose(file);
		return size;
	}
	cout << "Erreur. Taille de fichier introuvable." << endl;
	return -1;
}

int FileManager::createFile(string nameFile) {
	int fd = open(nameFile.c_str(), O_CREAT | O_WRONLY, 777);
	if (fd < 0) {
		cout << "Erreur lors de la création du fichier " << nameFile << endl;
		exit(0);
	}
	return fd;
}


void FileManager::sendPartFile(string nameFile, char* part, int length, Connexion* connexion) {
	Packet* p = new Packet("FDA", nameFile, length, part);
	if (p->getSizeData() == 0) {
		cout << "Erreur, data vide" << endl;
	}
	usleep(100*1000);
	connexion->sendPaquet(p);
	p->deleteFromMemory();
	free(p);
}


void FileManager::startSendFile(string nameFile, Connexion* connexion) {
	cout << "Envoi du fichier " << nameFile << " au client..." << endl;

	int descriptFichier = open(nameFile.c_str(), O_RDONLY);
	int size_read_eachTime = MAX_SIZE_PAQUETS - Packet::getSizeHeaders() - 100;

	if (size_read_eachTime <= 0) {
		cout << "ERREUR. size_read_eachTime est trop faible (" << size_read_eachTime << ")" << endl;
		close(descriptFichier);
		return;
	}

	cout << size_read_eachTime << " caractères seront lu par paquets" << endl;

	char* buffer;
	initBuffer(&buffer, size_read_eachTime);

	cout << "Envoi du fichier en cours..." << endl;
	int nbrRead = 0;
	while ((nbrRead = read(descriptFichier, buffer, size_read_eachTime))) {
		this->sendPartFile(nameFile, buffer, nbrRead, connexion);
		initBuffer(&buffer, size_read_eachTime, false);
	}

	close(descriptFichier);

	cout << "Fin d'envoi du fichier" << endl;
}

