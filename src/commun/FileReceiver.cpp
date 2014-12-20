#include "FileReceiver.h"

FileReceiver* FileReceiver::instance = NULL;

FileReceiver* FileReceiver::getInstance() {
	return FileReceiver::instance;
}

void FileReceiver::init(ProgressNotifier* p_n) {
	FileReceiver::instance = new FileReceiver(p_n);
}

FileReceiver::FileReceiver(ProgressNotifier* p_pn) {
	this->pn = p_pn;
	this->descs = new vector <int>();
	this->sizeFiles = new vector <double>();
	this->transfered = new vector <double>();
	this->listFiles = new vector <string>();
	nextKey = 0;
}

int FileReceiver::prepareTransfert(string nameFile, int size) {
	int fd = FileManager::getInstance()->createFileInDir(nameFile);
	if (fd > 0) {

		int idFile = this->nextKey++;

		this->descs->push_back(fd);
		this->sizeFiles->push_back((double) size);
		this->transfered->push_back(0.0);
		this->listFiles->push_back(nameFile);

		if (this->pn != NULL) {
			this->pn->onFileStart(nameFile);
		}
	} else {
		string s = "Erreur lors de la création du fichier " + nameFile;
		perror(s.c_str());
		stop();
	}
	return fd;
}

int FileReceiver::recvData(Packet* p) {
	string nameFile = p->getArgument();
	int lengthInt = p->getSizeData();
	char* datas = p->getDatas();
	int idFile = this->getKeyFromNameFile(nameFile);

	if (idFile < 0) {
		cout << "Erreur. Fichier introuvable dans la base des fichiers en cours de transfert." << endl;
		stop();
		return idFile;
	}

	int desc = this->descs->at(idFile);
	int totalSize = sizeFiles->at(idFile);
	
	if ((write(desc, datas, lengthInt))) {
		this->transfered->at(idFile) += (double) lengthInt;

		if (this->pn != NULL) {
			int pourcent = this->transfered->at(idFile) / totalSize * 100;
			this->pn->onFileProgress(nameFile, pourcent);
			if (this->transfered->at(idFile) >= this->sizeFiles->at(idFile)) {
				this->pn->onFileEnd(nameFile);
			}
		}
		return 1;
	}

	return 0;

}


int FileReceiver::getKeyFromNameFile(string nameFile) {
	for (int i = 0; i < this->listFiles->size(); ++i) {
		if (this->listFiles->at(i).compare(nameFile) == 0) {
			return i;
		}
	}
	return -1;
}

