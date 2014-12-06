#include "ClientConnexion.h"
#include "../commun/tools.h"

ClientConnexion::ClientConnexion(int socket) {
	this->sock = socket;
}

void* client_listeMessages(void* c) {
	ClientConnexion* cc = (ClientConnexion*) c;
	cc->listenMessages();
	return NULL;
}

void ClientConnexion::start_listenMessages() {
	pthread_t* id = (pthread_t*) malloc(sizeof(int)*2);
	pthread_create(id, NULL, client_listeMessages, (void *) this);
	ThreadManager::getInstance()->add(id);
}

void ClientConnexion::listenMessages() {
	char* buff;
	initBuffer(&buff, MAX_SIZE_PAQUETS);
	int retour;
	while ((retour = recv(this->sock, buff, MAX_SIZE_PAQUETS, 0)) > 0) {
		this->onPaquet(buff);
		initBuffer(&buff, MAX_SIZE_PAQUETS);
	}
	cout << "Fin d'attente de message." << endl;
}

int ClientConnexion::sendPaquet(Packet* p) {
	char* buffer = p->constructPacket();
	//p->display();
	//Packet::displayPacket(buffer, p->getSizePacket());
	int sock_err = send(this->sock, buffer, MAX_SIZE_PAQUETS, 0);
	return true;
}

void ClientConnexion::onPaquet(char* paquet) {
	//cout << "Paquet reçu du client : " << paquet << endl;
	
	Packet* p = new Packet(paquet);

	if (p->getId().compare("GET") == 0) {
		this->onPaquet_get(p->getArgument());
		return;
	}

	cout << "ERREUR. Packet non reconnu : " << paquet << endl;
	p->display();
}

void ClientConnexion::onPaquet_get(string nameFile) {
	cout << "Le client veut le fichier " << nameFile << endl;

	if (FileManager::getInstance()->exists(nameFile)) {
		cout << "Le fichier existe" << endl;
		this->sendPaquet(new Packet("RGE", "1", 0, NULL));

		int size = FileManager::getInstance()->getSize(nameFile);
		cout << "Taille du fichier = " << size << endl;
		this->sendPaquet(new Packet("FID", nameFile, size, NULL));
		this->startSendFile(nameFile);
	} else {
		cout << "Le fichier demandé n'existe pas" << endl;
		this->sendPaquet(new Packet("RGE", "0", 0, NULL));
	}
}


int cpt =0;

void ClientConnexion::sendPartFile(string nameFile, char* part, int length) {
	cpt++;
	Packet* p = new Packet("FDA", nameFile, length, part);
	//cout << "packet " << cpt << ". ";
	//p->display();
	if (p->getSizeData() == 0) {
		cout << "Erreur, data vide" << endl;
	}
	this->sendPaquet(p);
}

void ClientConnexion::startSendFile(string nameFile) {
	cout << "Envoi du fichier " << nameFile << " au client..." << endl;

	int descriptFichier = open(nameFile.c_str(), O_RDONLY);
	int size_read_eachTime = 10;
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
	while ( (nbrRead = read(descriptFichier, buffer, size_read_eachTime))) {
		this->sendPartFile(nameFile, buffer, nbrRead);
		initBuffer(&buffer, size_read_eachTime);
		usleep(100);
	}

	close(descriptFichier);

	cout << "Fin d'envoi du fichier" << endl;

}

