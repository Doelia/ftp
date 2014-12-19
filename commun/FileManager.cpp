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
	return FileManager::instance;
}

bool FileManager::exists(string nameFile) {
	string realName = this->dirFiles + nameFile;
	return this->keyExists(realName);
}

bool FileManager::keyExists(string nameFile) {
	string realName = this->dirFiles + nameFile;
	FILE *file;
	if ((file = fopen(realName.c_str(), "r"))) {
		fclose(file);
		return 1;
	}
	return 0;
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

int FileManager::createFile(string nameFile) {
	return this->createKey(this->dirFiles + nameFile);
}

int FileManager::createKey(string nameFile) {
	int fd = open(nameFile.c_str(), O_CREAT | O_WRONLY);
	if (fd < 0) {
		perror("Erreur lors de la création du fichier ");
		stop();
	}
	return fd;
}

bool FileManager::sendPartFile(string nameFile, char* part, int length, Connexion* connexion) {
	Packet* p = new Packet("FDA", nameFile, length, part);
	if (p->getSizeData() == 0) {
		cout << "Erreur, data vide" << endl;
	}
	usleep(10*1000);
	bool err = connexion->sendPaquet(p);
	p->deleteFromMemory();
	free(p);
	return err;
}

void* startSendFile_thread(void* pa) {
	struct t {
		string n;
		Connexion* c;
		FileManager* f;
	};

	t* v = (t*) pa;
	v->f->startSendFile_threaded(v->n, v->c);
}


void FileManager::startSendFile(string nameFile, Connexion* connexion) { 
	struct t {
		string n;
		Connexion* c;
		FileManager* f;
	};

	t* x = new t();
	x->n = nameFile;
	x->c = connexion;
	x->f = this;

	pthread_t* id = (pthread_t*) malloc(sizeof(int)*2);
	pthread_create(id, NULL, startSendFile_thread, (void*) x);
	ThreadManager::getInstance()->add(id);
}

void FileManager::startSendFile_threaded(string nameFile, Connexion* connexion) {
	cout << "Envoi du fichier " << nameFile << " au client..." << endl;

	string realNameFile = this->dirFiles + nameFile;
	int descriptFichier = open(realNameFile.c_str(), O_RDONLY);
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
	bool stop = false;
	while (!stop && (nbrRead = read(descriptFichier, buffer, size_read_eachTime))) {
		if (!this->sendPartFile(nameFile, buffer, nbrRead, connexion)) {
			cout << "Erreur lors du transfert." << endl;
			stop = true;
		}
		initBuffer(&buffer, size_read_eachTime, false);
	}

	close(descriptFichier);

	cout << "Fin d'envoi du fichier" << endl;
}

