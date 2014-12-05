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

int ClientConnexion::sendPaquet(string paquet) {
	cout << "Tentative d'envoi du paquet '" << paquet << "' au client" << endl;

	char* buffer;
	initBuffer(&buffer, MAX_SIZE_PAQUETS);
	int sizePaquet = paquet.size();

	if (sizePaquet >= MAX_SIZE_PAQUETS) {
		cout << "Erreur. Paquet trop gros (" << sizePaquet << "/" << MAX_SIZE_PAQUETS << ")" << endl;
		return false;
	}

	for (int i=0; i < sizePaquet; i++) {
		buffer[i] = paquet.at(i);
	}

	int sock_err = send(this->sock, buffer, MAX_SIZE_PAQUETS, 0);
	return true;
}

void ClientConnexion::onPaquet(string paquet) {
	cout << "Paquet reçu du client : '" << paquet << "'" << endl;
	vector<string> parts = split(paquet, DELI());
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
		this->sendPaquet("REP_GET"+DELI()+"1");

		int size = FileManager::getInstance()->getSize(nameFile);
		char sizeString[MAX_SIZE_PAQUETS];
		sprintf(sizeString, "%d", size);
		this->sendPaquet("FILE_HEAD"+DELI()+nameFile+DELI()+sizeString);
		this->startSendFile(nameFile);
	} else {
		cout << "Le fichier demandé n'existe pas" << endl;
		this->sendPaquet("REP_GET"+DELI()+"0");
	}
}

void ClientConnexion::sendPartFile(string nameFile, char* part, int length) {
	char lengthChar[21];
	sprintf(lengthChar, "%d", length);
	this->sendPaquet("FILE_DATA"+DELI()+nameFile+DELI()+lengthChar+DELI()+part);
}

void ClientConnexion::startSendFile(string nameFile) {
	cout << "Envoi du fichier " << nameFile << " au client..." << endl;

	int descriptFichier = open(nameFile.c_str(), O_RDONLY);
	int size_read_eachTime = MAX_SIZE_PAQUETS - 9 - DELI().size()*3 - nameFile.size() - 20;
	if (size_read_eachTime <= 0) {
		cout << "ERREUR. size_read_eachTime est trop faible (" << size_read_eachTime << ")" << endl;
		close(descriptFichier);
		return;
	}

	cout << "readEchTime=" << descriptFichier << endl;

	char* buffer;
	initBuffer(&buffer, size_read_eachTime);

	cout << "Envoi du fichier en cours..." << endl;
	int nbrRead = 0;
	while (nbrRead = read(descriptFichier, buffer, size_read_eachTime)) {
		this->sendPartFile(nameFile, buffer, nbrRead);
		initBuffer(&buffer, size_read_eachTime);
	}

	close(descriptFichier);

	cout << "Fin d'envoi du fichier" << endl;

}

