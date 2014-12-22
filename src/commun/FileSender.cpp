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
	//usleep(10*1000);
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

	int totalSize = FileManager::getInstance()->getSize(nameFile);
	cout << "size=" << totalSize << endl;
	if (this->pn != NULL) {
		this->pn->onFileStart(nameFile, totalSize, 1);
	}

	int descriptFichier = FileManager::getInstance()->openFileInDir(nameFile);
	int size_read_eachTime = MAX_SIZE_PAQUETS - Packet::getSizeHeaders() - 100;

	if (size_read_eachTime <= 0) {
		cout << "ERREUR. size_read_eachTime est trop faible (" << size_read_eachTime << ")" << endl;
		close(descriptFichier);
		return;
	}

	char* buffer;
	initBuffer(&buffer, size_read_eachTime);

	int nbrRead = 0;
	int totalSended = 0;
	bool stop = false;
	int lastPourcent = -1;
	while (!stop && (nbrRead = read(descriptFichier, buffer, size_read_eachTime))) {
		if (!this->sendPartFile(nameFile, buffer, nbrRead, connexion)) {
			cout << "Erreur lors du transfert." << endl;
			stop = true;
		} else {
			if (this->pn != NULL) {
				totalSended += nbrRead;
				int pourcent = totalSended / totalSize * 100;
				if (lastPourcent != pourcent) {
					this->pn->onFileProgress(nameFile, pourcent, 1);
					lastPourcent = pourcent;
				}
				
			}
			initBuffer(&buffer, size_read_eachTime, false);
		}
	}

	close(descriptFichier);

	if (this->pn != NULL) {
		this->pn->onFileEnd(nameFile, 1);
	}

}


