#include "ClientConnexion.h"

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

int ClientConnexion::sendPaquet(string paquet) {
	cout << "Tentative d'envoi du paquet '" << paquet << "' au client" << endl;

	char* buffer;
	initBuffer(&buffer, MAX_SIZE_PAQUETS);

	if (paquet.size() >= MAX_SIZE_PAQUETS) {
		cout << "Paquet trop gros" << endl;
		return false;
	}

	for (int i=0; i < paquet.size(); i++) {
		buffer[i] = paquet.at(i);
	}

	int sock_err = send(this->sock, buffer, MAX_SIZE_PAQUETS, 0);
	return true;
}

void ClientConnexion::onPaquet(string paquet) {
	cout << "Paquet reçu du client : '" << paquet << "'" << endl;
	vector<string> parts = split(paquet, ':');
	if (parts.size() == 0) {
		cout << "Erreur, paquet vide" << endl;
		return;
	}
	if (parts.at(0) == "GET") {
		this->onPaquet_get(parts.at(1));
	}
}

void ClientConnexion::onPaquet_get(string nameFile) {
	cout << "Le client veut le fichier " << nameFile << endl;

	if (FileManager::getInstance()->exists(nameFile)) {
		cout << "Le fichier existe" << endl;
		this->sendPaquet("REP_GET:1");

		int size = FileManager::getInstance()->getSize(nameFile);
		char sizeString[32];
		sprintf(sizeString, "%d", size);
		this->sendPaquet("FILE_HEAD:"+nameFile+":"+sizeString);
	} else {
		cout << "Le fichier demandé n'existe pas" << endl;
		this->sendPaquet("REP_GET:0");
	}
}

