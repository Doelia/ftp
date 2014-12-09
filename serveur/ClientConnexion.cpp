#include "ClientConnexion.h"
#include "../commun/tools.h"

ClientConnexion::ClientConnexion(int socket) {
	this->sock = socket;
}

void ClientConnexion::onPaquet(char* paquet, int size) {
	
	Packet* p = new Packet(paquet, size);

	if (p->getId().compare("GET") == 0) {
		this->onPaquet_get(p->getArgument());
		return;
	}

	cout << "ERREUR. Packet non reconnu : " << paquet << endl;
	p->display();

	p->deleteFromMemory();
	free(p);
}

void ClientConnexion::onPaquet_get(string nameFile) {
	cout << "Le client veut le fichier " << nameFile << endl;

	if (FileManager::getInstance()->exists(nameFile)) {
		cout << "Le fichier existe" << endl;
		this->sendPaquet(new Packet("RGE", "1", 0, NULL));

		int size = FileManager::getInstance()->getSize(nameFile);
		cout << "Taille du fichier = " << size << endl;
		this->sendPaquet(new Packet("FID", nameFile, size, NULL));
		FileManager::getInstance()->startSendFile(nameFile, this);
	} else {
		cout << "Le fichier demandé n'existe pas" << endl;
		this->sendPaquet(new Packet("RGE", "0", 0, NULL));
	}
}


