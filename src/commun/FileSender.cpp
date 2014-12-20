#include "FileSender.h"

FileSender* FileSender::instance = NULL;

FileSender* FileSender::getInstance() {
	if (FileSender::instance == NULL) {
		perror("Erreur, file sender non initialisé");
		stop();
	}
	return FileSender::instance;
}

void FileSender::init(ProgressNotifier* p_n) {
	FileSender::instance = new FileSender();
	FileSender::instance->pn = p_n;
}

FileSender::FileSender() {

}


bool FileSender::sendPartFile(string nameFile, char* part, int length, Connexion* connexion) {
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
		FileSender* f;
	};

	t* v = (t*) pa;
	v->f->startSendFile_threaded(v->n, v->c);

	return NULL;
}


void FileSender::startSendFile(string nameFile, Connexion* connexion) { 
	struct t {
		string n;
		Connexion* c;
		FileSender* f;
	};

	t* x = new t();
	x->n = nameFile;
	x->c = connexion;
	x->f = this;

	pthread_t* id = (pthread_t*) malloc(sizeof(int)*2);
	pthread_create(id, NULL, startSendFile_thread, (void*) x);
	ThreadManager::getInstance()->add(id);
}

void FileSender::startSendFile_threaded(string nameFile, Connexion* connexion) {
	cout << "Envoi du fichier " << nameFile << " au client..." << endl;

	int descriptFichier = FileManager::getInstance()->openFileInDir(nameFile);
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


