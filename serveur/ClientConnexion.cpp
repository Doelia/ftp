#include "ClientConnexion.h"
#include "../commun/tools.h"

ClientConnexion::ClientConnexion(int socket) {
	this->sock = socket;
}

void ClientConnexion::onPaquet(char* paquet, int size) {
	
	Packet* p = new Packet(paquet, size);

	if (p->getId().compare("GET") == 0) {
		this->onPaquet_get(p->getArgument());
	}

	else if (p->getId().compare("PUT") == 0) {
		this->onPaquet_put(p->getArgument(), p->getSizeData());
	}

	else if (p->getId().compare("FDA") == 0) { // Fide Data
		this->onPaquet_fileData(p);
	}

	else {
		cout << "ERREUR. Packet non reconnu : " << paquet << endl;
		p->display();
	}

	p->deleteFromMemory();
	free(p);
}

void ClientConnexion::onPaquet_get(string nameFile) {
	FileManager* fm = FileManager::getInstance();
	cout << "Le client veut le fichier " << nameFile << endl;

	if (fm->exists(nameFile)) {
		cout << "Le fichier existe" << endl;
		this->sendPaquet(new Packet("RGE", "1", 0, NULL));

		int size = fm->getSize(nameFile);
		cout << "Taille du fichier = " << size << endl;
		this->sendPaquet(new Packet("FID", nameFile, size, NULL));
		
		fm->startSendFile(nameFile, this);
	} else {
		cout << "Le fichier demandé n'existe pas" << endl;
		this->sendPaquet(new Packet("RGE", "0", 0, NULL));
	}
}

void ClientConnexion::onPaquet_fileData(Packet* p) {
	if (!FileReceiver::getInstance()->recvData(p)) {
		cout << "Erreur sur FileReceiver::getInstance()->recvData()" << endl;
	}
}

void ClientConnexion::onPaquet_put(string nameFile, int sizeFile) {
	cout << "onPaquet_put" << nameFile << ", size=" << sizeFile << endl;
	cout << "Le client veut envoyer le fichier " << nameFile << endl;

	if (FileManager::getInstance()->exists(nameFile)) {
		cout << "Le fichier existe déjà" << endl;
		this->sendPaquet(new Packet("CPU", nameFile, 0, NULL));
	} else {
		this->sendPaquet(new Packet("CPU", nameFile, 1, NULL));
		if (FileReceiver::getInstance()->prepareTransfert(nameFile, sizeFile) > 0) {

		} else {
			cout << "Erreur sur FileReceiver::getInstance()->prepareTransfert()" << endl;
		}
		}
}

